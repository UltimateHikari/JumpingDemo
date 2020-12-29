#pragma once
#include "gl_includes.hpp"

#include <memory>

#include "physics.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "light.hpp"


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
    PhysicalObject& getPhysical(){return *object;}
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
