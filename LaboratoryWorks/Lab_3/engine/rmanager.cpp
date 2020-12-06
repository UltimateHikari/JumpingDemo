#include "rmanager.hpp"

const std::string SHADER_PATH = "../shaders/";
const std::string MODEL_PATH = "../resources/";

using namespace glm;

ResourceManager* ResourceManager::instance_ = nullptr;

ResourceManager :: ResourceManager()
{
    if(instance_) throw std::runtime_error("reinstance");
    instance_ = this;
}

ResourceManager :: ~ResourceManager()
{
    instance_ = nullptr;
}

std::shared_ptr<Model> ResourceManager :: getModel(GLuint modelID) const
{
    return models[modelID];
}
std::shared_ptr<Light> ResourceManager :: getLight(GLuint lightID) const
{
    return lights[lightID];
}

GLuint ResourceManager :: getShadersAmount() const{
    return shaders.size();
}

GLuint ResourceManager :: getLightsAmount() const{
    return lights.size();
}

Shader& ResourceManager :: getShader(GLuint shaderID)
{
    return shaders[shaderID];
}

void ResourceManager :: loadResources(const std::string& config_file_path)
{
    std::ifstream fin(config_file_path);
    if(!fin.is_open()){
        std::cerr << "error opening resourcemanagerconfig\n";
        return;
    }
    int shaders_count, models_count, lights_count; 
    float x,y,z;
    std::string buffer_string, type_string;
    fin >> shaders_count >> models_count >> lights_count;
    for(int i = 0; i < shaders_count; ++i){
        fin >> buffer_string >> type_string;
        shaders.push_back(Shader(
            (SHADER_PATH + buffer_string + ".vs").c_str(),
            (SHADER_PATH + buffer_string + ".fs").c_str(),
            type_string
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
            std::shared_ptr<Light>(new DefaultLamp(vec3(x,y,z)))
        );
    }
}
