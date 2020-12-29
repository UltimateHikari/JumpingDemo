#include "window.hpp"
#include <iostream>

Window :: Window(GLuint width_, GLuint height_):
    height(height_), width(width_){
    if( !glfwInit() ){
		std::cerr << "Failed to initialize GLFW\n";
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow( width, height, "Lab3", NULL, NULL);
	if( window == NULL ){
		std::cerr << "Failed to open GLFW window.\n";
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW\n";
		glfwTerminate();
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Window :: ~Window(){
    glfwTerminate();
}

GLuint Window :: getWidth() const{
    return width;
}

GLuint Window :: getHeight() const{
    return height;
}

GLFWwindow * Window :: getWindow(){
    return window;
}