#pragma once
#include "gl_includes.hpp"
#include <memory>
#include "window.hpp"
#include "entity.hpp"


class CameraEntity{
    public:
        virtual void computeMatrices(float deltaTime) = 0;
        virtual glm::mat4 getViewMatrix() const = 0;
        virtual glm::mat4 getProjectionMatrix() const = 0;
        virtual glm::vec3 getPosition() const = 0;
        virtual ~CameraEntity(){};
};

// class FreeCamera : public CameraEntity{
//     private:
//         glm::vec3 position;
//         glm::vec3 direction;
//         glm::vec3 up;
//         float horizontalAngle;
//         float verticalAngle;
//         float speed;
//         float mouseSpeed;
//         float FoV;
//     public:
//         FreeCamera();
//         ~FreeCamera(){};
//         FreeCamera(
//             glm::vec3 position_, 
//             float hAngle,
//             float vAngle
//             );
//         void computeMatrices(GLFWwindow* window, float deltaTime);
//         glm::mat4 getViewMatrix() const;
//         glm::mat4 getProjectionMatrix() const;
//         glm::vec3 getPosition() const; //canditate to removal
//         float getVerticalAngle() const{ return verticalAngle;}
//         void setAngles(float horizontal, float vertical);
//         void setPosition(glm::vec3 position_);
// };

class TrackingCamera : public CameraEntity{
    private:
        glm::vec3 objectPosition;
        glm::vec3 direction;
        glm::vec3 up;
        float horizontalAngle;
        float verticalAngle;
        float FoV;
        float distance;
    public:
        TrackingCamera();
        ~TrackingCamera(){};
        TrackingCamera(
            glm::vec3 objectPosition_, 
            float distance_
            );
        void computeMatrices(float deltaTime);
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;
        glm::vec3 getPosition() const; //canditate to removal
        float getVerticalAngle() const{ return verticalAngle;}
        void setAngles(float horizontal, float vertical);
        void setPosition(glm::vec3 position_);
};

class ControllerInterface{
public:
    virtual ~ControllerInterface() = default;
    virtual void update(Window& window, float deltaTime) = 0;
};

class Player : public ControllerInterface{
private:
    TrackingCamera* camera;
    std::shared_ptr<Entity> entity;
    float speed;
    float angleSpeed;
    float verticalAngle;
    bool isActive;
    bool isInAir;
public:
    Player(
        TrackingCamera* camera_,
        std::shared_ptr<Entity> entity_
        ):  camera(camera_),
            entity(entity_),
            speed(400.0f),
            angleSpeed(0.003f),
            verticalAngle(0.0f),
            isActive(true),
            isInAir(false)
        {
        }
    void update(Window& window, float deltaTime);
};

class Roamer : public ControllerInterface{
private:
    std::shared_ptr<Entity> entity;
    glm::vec3 velocity;
    float accumulator;
    float volatility;
    float speed;
public:
    Roamer(
        std::shared_ptr<Entity> entity_,
        float volatility_
    ):
        entity(entity_),
        velocity(glm::vec3(0.0)),
        volatility(volatility_),
        accumulator(0.0f),
        speed(200.0f)
        {}
    void update(Window& window, float deltaTime); 
};