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

class PhysicalObject{
private:
    glm::vec3 position;
    glm::vec3 axis;
    float angle;
protected:
    void setPosition(glm::vec3 position_){ position = position_; }
    void setAngle(float angle_){ angle = angle_; }
    void setAxis(glm::vec3 axis_){ axis = axis_; }
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
    virtual void onForward(){}
    virtual void onBack(){}
    virtual void onRight(){}
    virtual void onLeft(){}
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

class CirculatingObject: public PhysicalObject{
private:
    glm::vec3 rvector;
    glm::vec3 cvector;
    float speed;
public:
    CirculatingObject(glm::vec3 position_, glm::vec3 axis_, float radius, float speed_): 
        cvector(position_), 
        rvector(radius*glm::normalize(glm::cross(glm::normalize(axis_), glm::normalize(glm::vec3(1.0f,0.7f,0.0f))))), 
        speed(speed_)
    {
        setPosition(position_);
        setAxis(axis_);
        std::cerr << to_string(rvector) <<std::endl;
    }
    void update(float deltaTime){
        float angle = getAngle();
        angle += deltaTime*speed;
        angle = fmod(angle,6.28f);class PhysicalObject{
private:
    glm::vec3 position;
    glm::vec3 axis;
    float angle;
protected:
    void setPosition(glm::vec3 position_){ position = position_; }
    void setAngle(float angle_){ angle = angle_; }
    void setAxis(glm::vec3 axis_){ axis = axis_; }
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
    virtual void onForward(){}
    virtual void onBack(){}
    virtual void onRight(){}
    virtual void onLeft(){}
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

class CirculatingObject: public PhysicalObject{
private:
    glm::vec3 rvector;
    glm::vec3 cvector;
    float speed;
public:
    CirculatingObject(glm::vec3 position_, glm::vec3 axis_, float radius, float speed_): 
        cvector(position_), 
        rvector(radius*glm::normalize(glm::cross(glm::normalize(axis_), glm::normalize(glm::vec3(1.0f,0.7f,0.0f))))), 
        speed(speed_)
    {
        setPosition(position_);
        setAxis(axis_);
        std::cerr << to_string(rvector) <<std::endl;
    }
    void update(float deltaTime){
        float angle = getAngle();
        angle += deltaTime*speed;
        angle = fmod(angle,6.28f);
        setAngle(angle);
        setPosition(cvector + glm::rotate(rvector, angle, getAxis()));
    }
};

class PlayerControlledObject : public PhysicalObject{
private:
    float speed;
    glm::vec3 direction;
    std::bitset<4> move_flags;
public:
    PlayerControlledObject(glm::vec3 position_): speed(3.0f), direction(glm::vec3(0.0)){
        setPosition(position_);
    }
    void update(float deltaTime){
        direction = glm::vec3(0.0f);
        float angle = getAngle();
        if(move_flags[0]){
            direction += glm::rotate(glm::vec3(1.0,0.0,0.0), angle, glm::vec3(0.0,1.0,0.0));
        }
        if(move_flags[1]){
            direction += glm::rotate(glm::vec3(-1.0,0.0,0.0), angle, glm::vec3(0.0,1.0,0.0));
        }
        if(move_flags[2]){
            angle -= speed*deltaTime;
        }
        if(move_flags[3]){
            angle += speed*deltaTime;
        }
        angle = fmod(angle, 6.28);
        setAngle(angle);
        setPosition(getPosition() + direction*speed*deltaTime);
        move_flags.reset();
    }
    void onForward(){
        move_flags[0] = true;
    }
    void onBack(){
        move_flags[1] = true;
    }
    void onRight(){
        move_flags[2] = true;
    }
    void onLeft(){
        move_flags[3] = true;
    }
};
        setAngle(angle);
        setPosition(cvector + glm::rotate(rvector, angle, getAxis()));
    }
};

class PlayerControlledObject : public PhysicalObject{
private:
    float speed;
    glm::vec3 direction;
    std::bitset<4> move_flags;
public:
    PlayerControlledObject(glm::vec3 position_): speed(3.0f), direction(glm::vec3(0.0)){
        setPosition(position_);
    }
    void update(float deltaTime){
        direction = glm::vec3(0.0f);
        float angle = getAngle();
        if(move_flags[0]){
            direction += glm::rotate(glm::vec3(1.0,0.0,0.0), angle, glm::vec3(0.0,1.0,0.0));
        }
        if(move_flags[1]){
            direction += glm::rotate(glm::vec3(-1.0,0.0,0.0), angle, glm::vec3(0.0,1.0,0.0));
        }
        if(move_flags[2]){
            angle -= speed*deltaTime;
        }
        if(move_flags[3]){
            angle += speed*deltaTime;
        }
        angle = fmod(angle, 6.28);
        setAngle(angle);
        setPosition(getPosition() + direction*speed*deltaTime);
        move_flags.reset();
    }
    void onForward(){
        move_flags[0] = true;
    }
    void onBack(){
        move_flags[1] = true;
    }
    void onRight(){
        move_flags[2] = true;
    }
    void onLeft(){
        move_flags[3] = true;
    }
};