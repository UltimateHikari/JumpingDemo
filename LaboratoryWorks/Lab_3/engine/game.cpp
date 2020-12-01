#include "game.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>

using namespace glm;

const std::string SHADER_PATH = "../shaders/";
const std::string MODEL_PATH = "../resources/";

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

    //std::cerr << "rendered"<< std::endl;

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

void ResourceManager :: loadResources(const std::string& config_file_path){
    std::ifstream fin(config_file_path);
    if(!fin.is_open()){
        std::cerr << "error opening resourcemanagerconfig\n";
        return;
    }
    int shaders_count, models_count, lights_count; 
    float x,y,z;
    std::string buffer_string;
    fin >> shaders_count >> models_count >> lights_count;
    for(int i = 0; i < shaders_count; ++i){
        fin >> buffer_string;
        shaders.push_back(Shader(
            (SHADER_PATH + buffer_string + ".vs").c_str(),
            (SHADER_PATH + buffer_string + ".fs").c_str()
            ));
    }
    for(int i = 0; i < models_count; ++i){
        fin >> buffer_string;
        models.push_back(
            std::shared_ptr<Model>(new Model(MODEL_PATH + buffer_string))
        );
    }
    for(int i = 0; i < lights_count; ++i){
        fin >> x >> y >> z;
        lights.push_back(
            std::shared_ptr<Light>(new DefaultLamp(vec3(4.0f,4.0f,2.0f)))
        );
    }

    /*for(auto i:shaders){
        std::cerr << i.getID() << " ";
    }
    std::cerr << std::endl;*/
}

void World :: update(float deltaTime){
    for(int i = 0; i < entities.size(); ++i){
        entities[i]->update(deltaTime);
    }
}

void World :: render(){
    ResourceManager& instance = ResourceManager::getInstance();
    int lights = instance.getLightsAmount();
    for(int i = 0; i < entities.size(); ++i){
        //excessive lightplacing (repeating shaders)
        Shader& shader = instance.getShader(entities[i]->getShaderID());
        shader.use();
        for(int j = 0; j < lights; ++j){
            instance.getLight(j)->place(shader);
        }
        shader.finalizeLight();

        entities[i]->render();
    }
}

void World :: addEntity(std::shared_ptr<Entity> entity){
    entities.push_back(entity);
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