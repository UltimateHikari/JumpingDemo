#include "entity.hpp"
#include "rmanager.hpp"

using namespace glm;

Entity :: Entity(
    GLuint model_id,
    GLuint shader_id,
    float scale_,
    PhysicalObject * object_
):  object(std::unique_ptr<PhysicalObject>(object_)), 
    model(ResourceManager::instance().getModel(model_id)),
    shader(ResourceManager::instance().getShader(shader_id)),
    scale_arg(scale_){}

Entity :: Entity(
        GLuint model_id,
        GLuint shader_id,
        float scale_,
        vec3 position
    ): Entity(model_id, shader_id, scale_, new StaticObject(position)){}

void Entity :: update(float deltaTime){
    object->update(deltaTime);
}

void Entity :: render(){
    shader.use();
    //shader.setMat4("model", mat4(1.0f));
    shader.setMat4("model", 
			translate(mat4(1.0f), object->getPosition())
            *rotate(mat4(1.0f), object->getAngle(), object->getAxis())
			*scale(mat4(1.0f), vec3(scale_arg)));
    model->Draw(shader);
}