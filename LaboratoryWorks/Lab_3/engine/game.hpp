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

#include <glm/gtx/string_cast.hpp>


class GraphicObject{
public:
    virtual void render() = 0;
};

class PhysicalObject{
private:
    glm::vec3 position;
    glm::vec3 axis;
    float angle;
protected:
    void setPosition(glm::vec3 position_){
        position = position_;
    }
    void setAngle(float angle_){
        angle = angle_;
    }
public:
    PhysicalObject(): 
        position(0.0f, 0.0f, 0.0f),
        axis(0.0f,1.0f,0.0f), 
        angle(0.0f)
        {};
    glm::vec3 getPosition(){return position;};
    glm::vec3 getAxis(){return axis;};
    float getAngle(){return angle;}
    virtual void update(float deltaTime) = 0;
};

class StaticObject : public PhysicalObject{
public:
    StaticObject(glm::vec3 position_){
        setPosition(position_);
    }
    void update(float deltaTime){}
};

class MovingObject : public PhysicalObject{
public:
    MovingObject(glm::vec3 position_){
        setPosition(position_);
    }
    void update(float deltaTime){
        setPosition(getPosition() += glm::vec3(deltaTime,0,0));
    }
};

class RotatingObject: public PhysicalObject{
public:
    RotatingObject(glm::vec3 position_){
        setPosition(position_);
    }
    void update(float deltaTime){
        float angle = getAngle();
        angle += deltaTime;
        angle = fmod(angle,6.28f);
        setAngle(angle);
    }
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