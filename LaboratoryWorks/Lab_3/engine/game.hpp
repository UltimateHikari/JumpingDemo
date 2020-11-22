#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <map>
#include <vector>

#include "model.hpp"
#include "shader.hpp"
#include "controls.hpp"
#include "window.hpp"
#include "light.hpp"

class GraphicObject{
public:
    virtual void render() = 0;
};

class PhysicalObject{
private:
    glm::vec3 position;
public:
    PhysicalObject(): position(0.0f, 0.0f, 0.0f){};
    virtual glm::vec3 getPosition() = 0;
    virtual void update(float deltaTime) = 0;
};

class Entity : public GraphicObject, public PhysicalObject{
private:
    std::shared_ptr<Model> model;
    GLuint shaderID;
    float scale_arg;
public:
    Entity(
        std::shared_ptr<Model> model_,
        GLuint shaderID_,
        float scale_
    ): model(model_), shaderID(shaderID_), scale_arg(scale_){};
    virtual ~Entity();
    virtual void render();
    GLuint getShaderID() const;
};

class LightEntity: public Entity{ //for future moving light-entities;
private:                        // now is simple entity
    std::shared_ptr<Light> light;
public:
    LightEntity(
        std::shared_ptr<Model> model_,
        GLuint shaderID_,
        float scale_,
        std::shared_ptr<Light> light_
    ): Entity(model_, shaderID_, scale_), light(light_){};
    //void update(float deltaTime) 
};

class ResourceManager {
private:
    std::vector< std::shared_ptr<Model> > models;
    std::vector< std::shared_ptr<Light> > lights;
    std::vector< Shader > shaders;
    ResourceManager(){};
public:
    static ResourceManager & getInstance();
    void loadResources(const std::string& config_file_path);
    std::shared_ptr<Model> getModel(GLuint modelID) const;
    std::shared_ptr<Light> getLight(GLuint lightID) const;
    GLuint getLightsAmount() const;
    Shader& getShader(GLuint shaderID);
};

class World{
private:
    std::vector<std::shared_ptr<Entity> > entities;
public:
    World() = default;
    void update(float deltaTime);
    void render();
    void addEntity(std::shared_ptr<Entity> entity);
};

class Game{
private:
    std::vector<std::unique_ptr<CameraEntity> > cameras;
    World world;
    Window& window;
    GLuint current_camera_index;
public:
    Game(Window& window_);
    void update();
    void render();
};