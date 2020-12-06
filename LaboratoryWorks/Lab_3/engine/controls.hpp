#ifndef CONTROLS_HPP
#define CONTROLS_HPP
#include <bitset>

class CameraEntity{
    public:
        virtual void computeMatricesFromInputs(GLFWwindow* window, float deltaTime) = 0;
        virtual glm::mat4 getViewMatrix() const = 0;
        virtual glm::mat4 getProjectionMatrix() const = 0;
        virtual glm::vec3 getPosition() const = 0;
        virtual ~CameraEntity(){};
        virtual void onUp() = 0;
        virtual void onDown() = 0;
        virtual void onRight() = 0;
        virtual void onLeft() = 0;
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
        std::bitset<4> move_flags;
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
        glm::vec3 getPosition() const;
        void onUp();
        void onDown();
        void onRight();
        void onLeft();
};

#endif