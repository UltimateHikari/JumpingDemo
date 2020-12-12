#pragma once
#include "gl_includes.hpp"
#include <memory>
#include "entity.hpp"


class CameraEntity{
    public:
        virtual void computeMatricesFromInputs(GLFWwindow* window, float deltaTime) = 0;
        virtual glm::mat4 getViewMatrix() const = 0;
        virtual glm::mat4 getProjectionMatrix() const = 0;
        virtual glm::vec3 getPosition() const = 0;
        virtual ~CameraEntity(){};
};

class FreeCamera : public CameraEntity{
    private:
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 up;
        float horizontalAngle;
        float verticalAngle;
        float speed;
        float mouseSpeed;
        float FoV;
    public:
        FreeCamera();
        ~FreeCamera(){};
        FreeCamera(
            glm::vec3 position_, 
            float hAngle,
            float vAngle
            );
        void computeMatricesFromInputs(GLFWwindow* window, float deltaTime);
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;
        glm::vec3 getPosition() const; //canditate to removal
        void setAngles(float horizontal, float vertical);
        void setPosition(glm::vec3 position_);
};

class ControllerInterface{
public:
    virtual ~ControllerInterface() = default;
    virtual void update(float deltaTime) = 0;
};

class Player{
private:
    std::shared_ptr<FreeCamera> camera;
    std::shared_ptr<Entity> entity;
    float speed;
    float angleSpeed;
    float verticalAngle;
public:
    Player(
        std::shared_ptr<FreeCamera> camera_,
        std::shared_ptr<Entity> entity_
        ): camera(camera_),
            entity(entity_),
            speed(3.0f),
            angleSpeed(0.005f),
            verticalAngle(0.0f)
        {}
    void update(float deltaTime);
};