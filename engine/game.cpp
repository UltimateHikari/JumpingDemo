#include "game.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <stdexcept>
#include <map>

using namespace glm;

void World :: update(float deltaTime){
    for(int i = 0; i < entities.size(); ++i){
        entities[i]->update(deltaTime);
    }
}

void World :: render(){
    for(int i = 0; i < entities.size(); ++i){
        entities[i]->render();
    }
}

void World :: addEntity(std::shared_ptr<Entity> entity){
    entities.push_back(entity);
}
void World :: prerender(){
    ResourceManager& instance = ResourceManager::instance();
    int lights = instance.getLightsAmount();
    //std::cerr << "placing lights: " << lights << std::endl;
    //excessive lightplacing (repeating shaders) and now only one
    Shader& shader = instance.getShader(0);
    shader.use();
    for(int j = 0; j < lights; ++j){
        instance.getLight(j)->place(shader);
    }
    shader.setVec3("material.specular", vec3(0.5f,0.5f,0.5f)); //need to move into resources
    shader.setFloat("material.shininess", 64.0f);   //and this
    shader.finalizeLight();
}

std::shared_ptr<Entity> World :: getPlayerEntity(GLuint index){
    return entities[index];
}

void Game :: useCamera(GLuint id){
    std::cerr << "using " << id << " camera\n";
    if(id >= cameras.size()) throw std::logic_error("unexistent camera");
    current_camera_index = id;
}

Game :: Game(Window& window_):  current_camera_index(0), window(window_){
    ResourceManager::instance().loadResources("../resource_config");
    //well, depth-test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glfwSetCursorPos(window.getWindow(), 1024/2, 768/2); // TODO parameters
    /**
     * manually push spawn some entities?
     * sceneLoader? nah, just hardcode stuff)
     */
    world.addEntity(std::make_shared<Entity>(3,0,20,new MovableObject(vec3(0.0f,2.0f,0.0f))));
    world.addEntity(std::make_shared<Entity>(1,0,30, vec3(0.0,-1.0,0.0)));
    world.addEntity(std::make_shared<Entity>(2,1,0.4, vec3(4.0f, 4.0f, 2.0f)));
    world.addEntity(std::make_shared<Entity>(2,1,0.4, vec3(-4.0f, 6.0f, -10.0f)));
    world.addEntity(std::make_shared<Entity>(2,1,0.4, vec3(4.0f, 10.0f, -6.0f)));
    world.addEntity(std::make_shared<Entity>(2,1,0.4, vec3(-6.0f, 12.0f, 8.0f)));
    world.addEntity(std::make_shared<Entity>(0,0,1,new MovableObject(
            vec3(10.0f,1.0f,0.0f))));
    world.addEntity(std::make_shared<Entity>(0,0,1,new MovableObject(
            vec3(6.0f,1.0f,6.0f))));
    world.addEntity(std::make_shared<Entity>(0,0,1,new MovableObject(
            vec3(3.0f,1.0f,-7.0f))));
    cameras.push_back(std::unique_ptr<CameraEntity>(new TrackingCamera));
    cameras.push_back(std::unique_ptr<CameraEntity>(new FreeCamera));
    controllers.push_back(std::unique_ptr<ControllerInterface>(
        new Player(
                static_cast<TrackingCamera*>(cameras[0].get()),
                world.getPlayerEntity(0),
                std::bind(
                    &Game::registerCallback, this, 
                    std::placeholders::_1, std::placeholders::_2
                    ),
                true
                )
        ));
    // controllers.push_back(std::unique_ptr<ControllerInterface>(
    //     new Spectator(
    //         static_cast<FreeCamera*>(cameras[0].get())
    //     )
    // ));
    controllers.push_back(std::unique_ptr<ControllerInterface>(
        new CameraCycler(
            controllers[0].get(),
            std::bind(&Game::useCamera, this, 0),
            std::bind(
                    &Game::registerCallback, this, 
                    std::placeholders::_1, std::placeholders::_2
                    )
                )
        ));
    // dynamic_cast<CameraCycler*>(controllers[2].get())->registerCameraController(
    //     std::bind(&Game::useCamera, this, 1),
    //     controllers[1].get()
    // );
    controllers.push_back(std::unique_ptr<ControllerInterface>(
        new Roamer(
                world.getPlayerEntity(6),
                8.0f
                )
        ));
    controllers.push_back(std::unique_ptr<ControllerInterface>(
        new Roamer(
                world.getPlayerEntity(7),
                17.0f
                )
        ));
    controllers.push_back(std::unique_ptr<ControllerInterface>(
        new Roamer(
                world.getPlayerEntity(8),
                3.0f
                )
        ));
    setCallbacks();
    world.prerender();
}

void Game :: setCallbacks(){
    glfwSetWindowUserPointer(window.getWindow(), this); //reee evil magic with vacant pointer
    glfwSetKeyCallback(
        window.getWindow(), 
        [](
            GLFWwindow* window, 
            int key, int scancode, 
            int action, int mods
            )
            {
                auto& self = *static_cast<Game*>(glfwGetWindowUserPointer(window));
                for(auto i:self.callbacks){
                    if(key == i.first && action == GLFW_PRESS)
                        i.second();
                }
            });
}

void Game :: registerCallback(int keyId, std::function<void()> foo){
    callbacks[keyId] = foo; //mb an exception for overwrite?
}


void Game :: update(){
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
	float deltaTime = static_cast<float>(currentTime - lastTime);
    /** 
     * here should be some accumulator stuff for reactphysics3d?
     * 'cause it wants fixed time intervals for updates 
     */
    for(int i = 0; i < controllers.size(); ++i){
        controllers[i]->update(window,deltaTime);
    }
    cameras[current_camera_index]->computeMatrices(deltaTime);
    for(int i = 0; i < ResourceManager::instance().getShadersAmount(); ++i){
        Shader& shader = ResourceManager::instance().getShader(i);
        shader.use();
        shader.setMat4("projection", cameras[current_camera_index]->getProjectionMatrix());
        shader.setMat4("view", cameras[current_camera_index]->getViewMatrix());
        if(shader.getType() == "material"){
            shader.setVec3("viewPosition", cameras[current_camera_index]->getPosition());
        }
        if(shader.getType() == "cubemap"){
            shader.setMat4("view", mat4(mat3(cameras[current_camera_index]->getViewMatrix())));
        }
    }
    world.update(deltaTime);

    lastTime = currentTime;
}

void Game :: render(){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world.render();
    ResourceManager::instance().getSkybox()->Draw();
}