#ifndef CONTROLS_HPP
#define CONTROLS_HPP

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
        glm::vec3 getPosition() const;
};

#endif