#pragma once
#include "gl_includes.hpp"
#include <memory>
#include <map>
#include "window.hpp"
#include "entity.hpp"
#include <functional>

#define JUMP_TYPES_COUNT 3

class CameraEntity{
    public:
        virtual void computeMatrices(float deltaTime) = 0;
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
        void computeMatrices(GLFWwindow* window, float deltaTime);
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;
        glm::vec3 getPosition() const;
        float getVerticalAngle() const{ return verticalAngle;}
        void setAngles(float horizontal, float vertical);
        void setPosition(glm::vec3 position_);
};

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
        glm::vec3 getPosition() const; 
        float getVerticalAngle() const{ return verticalAngle;}
        void setAngles(float horizontal, float vertical);
        void setPosition(glm::vec3 position_);
};

class ControllerInterface{
public:
    virtual ~ControllerInterface() = default;
    virtual void update(Window& window, float deltaTime) = 0;
    virtual void receiveCallback(int id){};
    virtual void enable(){};
    virtual void disable(){};
};

class JumpTechnique{
public:
    virtual ~JumpTechnique() = default;
    virtual glm::vec3 jump(PhysicalObject* physical) = 0;
    virtual void reset() = 0;
    virtual void refresh(PhysicalObject* physical, float deltaTime){} 
};

class DoubleJump : public JumpTechnique{
private:
    const int defaultJumpCharges = 2;
    int jumpChargesLeft;
public:
    DoubleJump(): jumpChargesLeft(defaultJumpCharges){}
    glm::vec3 jump(PhysicalObject* physical);
    void reset();
};

class LiftJump : public JumpTechnique{
private:
    const int defaultJumpCharges = 2;
    int jumpChargesLeft;
    float liftActiveTime;
    bool hadJumped;
    const float maxLiftTime = 0.5f;
public:
    LiftJump(): 
        jumpChargesLeft(defaultJumpCharges), 
        liftActiveTime(maxLiftTime),
        hadJumped(false){}
    glm::vec3 jump(PhysicalObject* physical);
    void reset();
    void refresh(PhysicalObject* physical, float deltaTime);
};

class GlideJump : public JumpTechnique{
private:
    const int defaultJumpCharges = 2;
    int jumpChargesLeft;
    float glideActiveTime;
    bool hadJumped;
    const float maxGlideTime = 1.5f;
public:
    GlideJump():
        jumpChargesLeft(defaultJumpCharges),
        glideActiveTime(maxGlideTime),
        hadJumped(false){}
    glm::vec3 jump(PhysicalObject* physical);
    void reset();
    void refresh(PhysicalObject* physical, float deltaTime);
};

// class CameraCycler : public ControllerInterface{
// private:
//     ControllerInterface* currentHandler;
// public:
//     CameraCycler(ControllerInterface* activeHandler):
//         currentHandler(activeHandler){}
//     void addCameraController(GLuint id, ControllerInterface* cameraHandler);
// };

enum class Callbacks{NextJump, Jump};

class Player : public ControllerInterface{
private:
    TrackingCamera* camera;
    std::shared_ptr<Entity> entity;
    std::vector<std::unique_ptr<JumpTechnique> >jumps;
    float speed;
    float angleSpeed;
    float verticalAngle;
    bool isInAir;
    bool hadJumped;
    int jumpType;
public:
    Player(
        TrackingCamera* camera_,
        std::shared_ptr<Entity> entity_,
        std::function<void(int, std::function<void()> ) > addCallback
        ):  camera(camera_),
            entity(entity_),
            speed(400.0f),
            angleSpeed(0.003f),
            verticalAngle(0.0f),
            isInAir(false),
            hadJumped(false),
            jumpType(0)
        {
            addCallback(GLFW_KEY_SPACE, 
                std::bind(&Player::receiveCallback, this, static_cast<int>(Callbacks::Jump)));
            addCallback(GLFW_KEY_V, 
                std::bind(&Player::receiveCallback, this, static_cast<int>(Callbacks::NextJump)));

            jumps.push_back(std::unique_ptr<JumpTechnique>(new DoubleJump()));
            jumps.push_back(std::unique_ptr<JumpTechnique>(new LiftJump()));
            jumps.push_back(std::unique_ptr<JumpTechnique>(new GlideJump()));
        }
    void update(Window& window, float deltaTime);
    void receiveCallback(int id);
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