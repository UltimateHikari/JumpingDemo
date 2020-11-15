#include "game.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

void Entity :: render(){
    Shader& shader = ResourceManager::getInstance().getShader(shaderID);
    shader.use();
    shader.setMat4("model", 
			translate(mat4(1.0f), getPosition())
			*scale(mat4(1.0f), vec3(scale_arg)));

    model->Draw(shader);
}

GLuint Entity :: getShaderID() const{
    return shaderID;
}

std::shared_ptr<Model> ResourceManager :: getModel(GLuint modelID) const{
    return models[modelID];
}
std::shared_ptr<Light> ResourceManager :: getLight(GLuint lightID) const{
    return lights[lightID];
}

GLuint ResourceManager :: getLightsAmount() const{
    return lights.size();
}

Shader& ResourceManager :: getShader(GLuint shaderID){
    return shaders[shaderID];
}

ResourceManager& ResourceManager :: getInstance(){
    static ResourceManager manager;
    return manager;
}

// void ResourceManager :: loadResources(const std::string& config_file_path){

// }

void World :: update(float deltaTime){
    for(auto i : entities){
        i.update(deltaTime);
    }
}

void World :: render(){
    for(auto i : entities){
        i.render();
    }
}

void World :: spawnEntity(Entity && entity){
    entities.push_back(std::move(entity));
}

Game :: Game(Window& window_):  current_camera_index(0), window(window_){
    ResourceManager::getInstance().loadResources("../resource_config");
    cameras.push_back(FreeCamera());
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
}

void Game :: update(){
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
	float deltaTime = static_cast<float>(currentTime - lastTime);
    /** 
     * here should be some accumulator stuff for reactphysics3d
     * 'cause it wants fixed time intervals for updates 
     */
    cameras[current_camera_index].computeMatricesFromInputs(window.getWindow(), deltaTime);

    for(auto i : world.entities){
        i.update(deltaTime);
    }

    lastTime = currentTime;
}

void Game :: render(){
    ResourceManager& instance = ResourceManager::getInstance();
    int lights = instance.getLightsAmount();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(auto i : world.entities){
        //excessive lightplacing (repeating shaders)
        Shader& shader = instance.getShader(i.getShaderID());
        for(int j = 0; j < lights; ++j){
            instance.getLight(j)->place(shader);
        }
        shader.finalizeLight();
        i.render();
    }
}