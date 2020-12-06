#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <map>
#include <vector>

#include "rmanager.hpp"
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
    virtual glm::vec3 getPosition(){return position;};
    virtual void update(float deltaTime) = 0;
};

class Entity : public GraphicObject, public PhysicalObject{
private:
    std::shared_ptr<Model> model;
    Shader& shader;
    float scale_arg;
public:
    Entity(
        std::shared_ptr<Model> model_,
        Shader& shader_,
        float scale_
    ): model(model_), shader(shader_), scale_arg(scale_){};
    Entity(
        GLuint model_id,
        GLuint shader_id,
        float scale_
    );
    virtual ~Entity() = default;
    virtual void render();
    virtual void update(float deltaTime){};
};

class LightEntity: public Entity{ //for future moving light-entities;
private:                        // now is simple entity
    std::shared_ptr<Light> light;
public:
    LightEntity(
        std::shared_ptr<Model> model_,
        Shader& shader_,
        float scale_,
        std::shared_ptr<Light> light_
    ): Entity(model_, shader_, scale_), light(light_){};
    virtual ~LightEntity() = default;
    //void update(float deltaTime) 
};

class World{
private:
    std::vector<std::shared_ptr<Entity> > entities;
public:
    World() = default;
    void update(float deltaTime);
    void prerender();
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