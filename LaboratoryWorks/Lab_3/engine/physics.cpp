#include "physics.hpp"

using namespace glm;

void PhysicalObject :: setPosition(vec3& position_)
{
    position = position_;
}

void PhysicalObject :: setAngle(float angle_)
{ 
    angle = angle_; 
}

void PhysicalObject :: setAxis(vec3& axis_)
{
    axis = axis_;
}

const vec3& PhysicalObject :: getPosition() const 
{
    return position;
}
const vec3& PhysicalObject :: getAxis() const 
{
    return axis;
}
float PhysicalObject :: getAngle() const
{
    return angle;
}

void PhysicalObject :: applyGravitation()
{   
    gravitation += 0.002f;
    position.y -= gravitation;
    if(position.y < 0.0){
        position.y = 0.0f;
        gravitation = 0.0f;
    }
}

void MovableObject :: doTurn(float angle_)
{
    deltaAngle = angle_;
}

void MovableObject :: doMove(vec3& velocity_)
{
    velocity = velocity_;
}

void MovableObject :: update(float deltaTime)
{
    setAngle(fmod(getAngle() + deltaAngle, 6.28)); //im too lazy for precision
    vec3 dPosition = getPosition() + velocity*deltaTime;
    setPosition(dPosition);
    //mb acceleration after
    velocity = vec3(0.0f);
    deltaAngle = 0.0f;
    applyGravitation();
}