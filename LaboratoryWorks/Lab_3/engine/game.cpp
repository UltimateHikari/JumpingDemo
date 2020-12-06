#include "game.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>

using namespace glm;

Entity :: Entity(
    GLuint model_id,
    GLuint shader_id,
    float scale_,
    PhysicalObject * object_
):  object(std::unique_ptr<PhysicalObject>(object_)), 
    model(ResourceManager::instance().getModel(model_id)),
    shader(ResourceManager::instance().getShader(shader_id)),
    scale_arg(scale_){}

Entity :: Entity(
        GLuint model_id,
        GLuint shader_id,
        float scale_,
        vec3 position
    ): Entity(model_id, shader_id, scale_, new StaticObject(position)){}

void Entity :: update(float deltaTime){
    object->update(deltaTime);
}

void Entity :: render(){
    shader.use();
    //shader.setMat4("model", mat4(1.0f));
    shader.setMat4("model", 
			translate(mat4(1.0f), object->getPosition())
            *rotate(mat4(1.0f), object->getAngle(), object->getAxis())
			*scale(mat4(1.0f), vec3(scale_arg)));
    model->Draw(shader);
}

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

Game :: Game(Window& window_):  current_camera_index(0), window(window_){
    ResourceManager::instance().loadResources("../resource_config");
    cameras.push_back(std::unique_ptr<CameraEntity>(new FreeCamera));
    //well, depth-test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glfwSetCursorPos(window.getWindow(), 1024/2, 768/2); // TODO parameters

    /**
     * manually push spawn some entities?
     * sceneLoader?
     */
    world.addEntity(std::make_shared<Entity>(0,0,1,new CirculatingObject(
            vec3(-4.0f,2.0f,-4.0f),
            vec3(1.0f,0.0f,0.0f),
            4.0f, 2.0f)));
    world.addEntity(std::make_shared<Entity>(0,0,1,new CirculatingObject(
            vec3(0.0f,2.0f,0.0f),
            vec3(0.1f,1.0f,0.0f),
            4.0f, 2.0f)));
    world.addEntity(std::make_shared<Entity>(0,0,1,new PlayerControlledObject(vec3(0.0f,1.0f,0.0f))));
    world.addEntity(std::make_shared<Entity>(1,0,40, vec3()));
    world.addEntity(std::make_shared<Entity>(2,1,0.4, vec3(4.0f, 4.0f, 2.0f)));
    world.addEntity(std::make_shared<Entity>(2,1,0.4, vec3(-4.0f, 6.0f, -10.0f)));
    world.prerender();
}

void Game :: use_events(){
    if(glfwGetKey( window.getWindow(), GLFW_KEY_W ) == GLFW_PRESS){
        world.getPlayerEntity(2)->onForward();
    }
    if(glfwGetKey( window.getWindow(), GLFW_KEY_S ) == GLFW_PRESS){
        world.getPlayerEntity(2)->onBack();
    }
    if(glfwGetKey( window.getWindow(), GLFW_KEY_D ) == GLFW_PRESS){
        world.getPlayerEntity(2)->onRight();
    }
    if(glfwGetKey( window.getWindow(), GLFW_KEY_A ) == GLFW_PRESS){
        world.getPlayerEntity(2)->onLeft();
    }
    if(glfwGetKey( window.getWindow(), GLFW_KEY_UP ) == GLFW_PRESS){
        cameras[current_camera_index]->onUp();
    }
    if(glfwGetKey( window.getWindow(), GLFW_KEY_DOWN ) == GLFW_PRESS){
        cameras[current_camera_index]->onDown();
    }
    if(glfwGetKey( window.getWindow(), GLFW_KEY_RIGHT ) == GLFW_PRESS){
        cameras[current_camera_index]->onRight();
    }
    if(glfwGetKey( window.getWindow(), GLFW_KEY_LEFT ) == GLFW_PRESS){
        cameras[current_camera_index]->onLeft();
    }
}

void Game :: update(){
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
	float deltaTime = static_cast<float>(currentTime - lastTime);
    /** 
     * here should be some accumulator stuff for reactphysics3d?
     * 'cause it wants fixed time intervals for updates 
     */
    cameras[current_camera_index]->computeMatricesFromInputs(window.getWindow(), deltaTime);
    for(int i = 0; i < ResourceManager::instance().getShadersAmount(); ++i){
        Shader& shader = ResourceManager::instance().getShader(i);
        shader.use();
        shader.setMat4("projection", cameras[current_camera_index]->getProjectionMatrix());
        shader.setMat4("view", cameras[current_camera_index]->getViewMatrix());
        if(shader.getType() == "material")
            shader.setVec3("viewPosition", cameras[current_camera_index]->getPosition());
    }

    world.update(deltaTime);

    lastTime = currentTime;
}

void Game :: render(){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world.render();
}