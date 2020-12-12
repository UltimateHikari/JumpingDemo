#pragma once
#include "gl_includes.hpp"


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