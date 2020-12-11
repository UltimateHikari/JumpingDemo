#include "rmanager.hpp"

const std::string SHADER_PATH = "../shaders/";
const std::string MODEL_PATH = "../resources/";
const std::string SKYBOX_PATH = "../resources/skybox/";

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

std::shared_ptr<Skybox> ResourceManager :: getSkybox() const{
    return skybox;
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
    //skyboxload
    std::vector<std::string> faces(6);
    for(int i = 0; i < 6; ++i){
        fin >> faces[i];
    }
    GLuint SkyboxTexture;
    glGenTextures(1, &SkyboxTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTexture);
 
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load((SKYBOX_PATH + faces[i]).c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cerr << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    fin.close();
    std::cerr << "Skybox texture loaded\n";
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    std::ifstream fsk(SKYBOX_PATH + "skybox.obj");
    if(!fsk.is_open()){
        std::cerr << "error opening skyboxobj\n";
        return;
    }

    std::vector<float> vertices(36*3);
    for(int i = 0; i < 36*3; ++i){
        fsk >> vertices[i];
    }
    skybox = std::make_shared<Skybox>(vertices, shaders[2], SkyboxTexture);
    fsk.close();
}