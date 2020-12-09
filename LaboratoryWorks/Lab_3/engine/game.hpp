#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <bitset>

#include "rmanager.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "controls.hpp"
#include "window.hpp"
#include "light.hpp"
#include "physics.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>


class GraphicObject{
public:
    virtual void render() = 0;
};

class Entity : public GraphicObject{
private:
    std::shared_ptr<Model> model;
    Shader& shader;
    float scale_arg;
    std::unique_ptr<PhysicalObject> object;
public:
    Entity(
        GLuint model_id,
        GLuint shader_id,
        float scale_,
        PhysicalObject* object_
    );
    Entity(
        GLuint model_id,
        GLuint shader_id,
        float scale_,
        glm::vec3 position = glm::vec3()
    );
    virtual ~Entity() = default;
    virtual void render();
    void update(float deltaTime);
    void onForward(){object->onForward();};
    void onBack(){object->onBack();};
    void onRight(){object->onRight();};
    void onLeft(){object->onLeft();};
};

// class LightEntity: public Entity{ //for future moving light-entities;
// private:                        // now is simple entity
//     std::shared_ptr<Light> light;
// public:
//     LightEntity(
//         std::shared_ptr<Model> model_,
//         Shader& shader_,
//         float scale_,
//         glm::vec3 position,
//         std::shared_ptr<Light> light_
//     ): Entity(model_, shader_, scale_, position), light(light_){};
//     virtual ~LightEntity() = default;
//     //void update(float deltaTime) 
// };

class World{
private:
    std::vector<std::shared_ptr<Entity> > entities;
public:
    World() = default;
    void update(float deltaTime);
    void prerender();
    void render();
    void addEntity(std::shared_ptr<Entity> entity);
    std::shared_ptr<Entity> getPlayerEntity(GLuint index);
};

class Game{
private:
    std::vector<std::unique_ptr<CameraEntity> > cameras;
    World world;
    Window& window;
    GLuint current_camera_index;
public:
    Game(Window& window_);
    void use_events();
    void update();
    void render();
};