#include "game.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>

using namespace glm;

Entity :: Entity(
        GLuint model_res_id,
        GLuint shader_res_id,
        float scale_
    ): Entity(
        ResourceManager::getInstance().getModel(model_res_id),
        ResourceManager::getInstance().getShader(shader_res_id).getID(),
        scale_
    ){}

void Entity :: render(){
    Shader& shader = ResourceManager::getInstance().getShader(shaderID);
    shader.use();
    shader.setMat4("model", mat4(1.0f));
    // shader.setMat4("model", 
	// 		translate(mat4(1.0f), getPosition())
	// 		*scale(mat4(1.0f), vec3(scale_arg)));

    //std::cerr << "rendered to " << shader.getID() << std::endl;
    model->Draw(shader);
}

GLuint Entity :: getShaderID() const{
    return shaderID;
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
    ResourceManager& instance = ResourceManager::getInstance();
    int lights = instance.getLightsAmount();
    //excessive lightplacing (repeating shaders) and now only one
        Shader& shader = instance.getShader(entities[0]->getShaderID());
        shader.use();
        for(int j = 0; j < lights; ++j){
            instance.getLight(j)->place(shader);
        }
        shader.finalizeLight();

}

Game :: Game(Window& window_):  current_camera_index(0), window(window_){
    ResourceManager::getInstance().loadResources("../resource_config");
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
    world.addEntity(std::make_shared<Entity>(0,0,1));
    world.prerender();
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

    world.update(deltaTime);

    lastTime = currentTime;
}

void Game :: render(){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world.render();
}