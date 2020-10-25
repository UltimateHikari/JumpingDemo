#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class Window {
private:
    GLuint width;
    GLuint height;
    GLFWwindow * window;
public:
    Window(GLuint width_, GLuint height_);
    ~Window();
    GLuint getWidth() const;
    GLuint getHeight() const;
    GLFWwindow * getWindow();
};