#ifndef CONTROLS_HPP
#define CONTROLS_HPP

class CameraEntity{
    public:
        virtual void computeMatricesFromInputs(GLFWwindow* window) = 0;
        virtual glm::mat4 getViewMatrix() const = 0;
        virtual glm::mat4 getProjectionMatrix() const = 0;
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
        FreeCamera(
            glm::vec3 position_, 
            float hAngle,
            float vAngle
            );
        void computeMatricesFromInputs(GLFWwindow* window);
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;
};

#endif