#pragma once
#include "gl_includes.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>

class PhysicalObject{
private:
    glm::vec3 position;
    glm::vec3 axis;
    float angle;
protected:
    void setPosition(glm::vec3& position_);
    void setAngle(float angle_);
    void setAxis(glm::vec3& axis_);
public:
    PhysicalObject(): 
        position(glm::vec3(0.0f)),
        axis(glm::vec3(0.0f,1.0f,0.0f)), 
        angle(0.0)
        {};
    virtual ~PhysicalObject() = default;
    const glm::vec3& getPosition() const;
    const glm::vec3& getAxis() const;
    float getAngle() const;
    virtual void doTurn(float angle_){};
    virtual void doMove(glm::vec3& velocity_){}
    virtual void update(float deltaTime) = 0;
};

class StaticObject : public PhysicalObject{
public:
    StaticObject(glm::vec3 position_)
    {
        setPosition(position_);
    }
    virtual void update(float deltaTime){} //not going anywhere, mb physics in future
};

class MovableObject : public PhysicalObject{
private:
    glm::vec3 velocity;
    float deltaAngle;
public:
    MovableObject(glm::vec3&& position_): 
        velocity(glm::vec3(0.0f)), 
        deltaAngle(0.0)
    {
        setPosition(position_);
    }
    void doTurn(float angle_);
    void doMove(glm::vec3& velocity_);
    virtual void update(float deltaTime);
};