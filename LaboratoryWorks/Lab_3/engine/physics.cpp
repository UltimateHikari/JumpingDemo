#include "physics.hpp"
#include <iostream>

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

float PhysicalObject :: getG()
{
    return g;
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

void MovableObject :: applyGravitation(float deltaTime)
{   
    velocity.y -= getG()*deltaTime;
}

void PhysicalObject :: disableGravitation()
{
    g = 0.0f;
}

void PhysicalObject :: enableGravitation()
{
    g = 8.0f;
}

void MovableObject :: doTurn(float angle_)
{
    deltaAngle = angle_;
}

void MovableObject :: doMove(vec3& velocity_)
{
    velocity.x = velocity_.x;
    velocity.y = (velocity_.y < 0.1 ? velocity.y : velocity_.y);
    velocity.z = velocity_.z;
}

void MovableObject :: correctPosition(){
    vec3 position = getPosition();
    if(position.y < 0.0f){
        position.y = 0.0f;
        velocity.y = 0.0f;
        enableGravitation();
    }
    setPosition(position);
}

void MovableObject :: update(float deltaTime)
{
    setAngle(fmod(getAngle() + deltaAngle, 6.28)); //im too lazy for precision
    vec3 dPosition = getPosition() + velocity*deltaTime;
    setPosition(dPosition);
    correctPosition();
    //mb acceleration after
    velocity = vec3(0.0f, velocity.y, 0.0f);
    deltaAngle = 0.0f;
    applyGravitation(deltaTime);
}