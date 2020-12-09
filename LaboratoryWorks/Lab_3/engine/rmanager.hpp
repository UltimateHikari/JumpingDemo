#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <vector>

#include "model.hpp"
#include "shader.hpp"
#include "controls.hpp"
#include "light.hpp"


class ResourceManager {
private:
    std::vector< std::shared_ptr<Model> > models;
    std::vector< std::shared_ptr<Light> > lights;
    std::vector< Shader > shaders;
    std::shared_ptr<Skybox> skybox;
    static ResourceManager* instance_;
public:
    ResourceManager();
    ~ResourceManager();
    static ResourceManager & instance(){ return *instance_; }
    void loadResources(const std::string& config_file_path);
    std::shared_ptr<Model> getModel(GLuint modelID) const;
    std::shared_ptr<Light> getLight(GLuint lightID) const;
    std::shared_ptr<Skybox> getSkybox() const;
    GLuint getShadersAmount() const;
    GLuint getLightsAmount() const;
    Shader& getShader(GLuint shaderID);
};