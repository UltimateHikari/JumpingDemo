#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <stdlib.h>

#include "shader.hpp"
#include "controls.hpp"
#include "window.hpp"
#include "model.hpp"
#include "light.hpp"

#include "rmanager.hpp"
#include "game.hpp"

#include <glm/gtx/string_cast.hpp>


int main( void ){
	Window w(1024,768);
	ResourceManager manager;
	Game game(w);
	// manager.loadResources("../resource_config");

	// Shader& shader = ResourceManager::instance().getShader(0);
	// Shader& lightShader = ResourceManager::instance().getShader(1);

	// //well, depth-test
	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LESS);
	// // Cull triangles which normal is not towards the camera
	// glEnable(GL_CULL_FACE);
	// //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// //Spawn some cameras
	// FreeCamera camera;
	// glfwSetCursorPos(w.getWindow(), 1024/2, 768/2); // TODO parameters

	// shader.use();

	// ResourceManager::instance().getLight(0)->place(shader);	
	// ResourceManager::instance().getLight(1)->place(shader);
	// shader.finalizeLight();

	do{
		game.update();
		game.render();
		// glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// shader.use();

		// camera.computeMatricesFromInputs(w.getWindow(),1.0f);
		// shader.setMat4("projection", camera.getProjectionMatrix());
		// shader.setMat4("view", camera.getViewMatrix());
		// shader.setMat4("model", 
		// 	translate(mat4(1.0f), vec3(0.0f,2.0f,0.0f))); 

		// shader.setVec3("material.specular", vec3(0.5f,0.5f,0.5f));
		// shader.setFloat("material.shininess", 64.0f);
		// shader.setVec3("viewPosition", camera.getPosition());

		// ResourceManager::instance().getModel(0)->Draw(shader);

		// shader.setMat4("model", 
		// 	scale(mat4(1.0f), vec3(40.0f)));

		// ResourceManager::instance().getModel(1)->Draw(shader);

		// lightShader.use();
		// lightShader.setMat4("projection", camera.getProjectionMatrix());
		// lightShader.setMat4("view", camera.getViewMatrix());
		// lightShader.setMat4("model", 
		// 	translate(mat4(1.0f), vec3(4.0f,4.0f,2.0f))
		// 	*scale(mat4(1.0f), vec3(0.5f)));
		// ResourceManager::instance().getModel(2)->Draw(lightShader);
		// lightShader.setMat4("model", 
		// 	translate(mat4(1.0f), vec3(-4.0f,6.0f,-10.0f))
		// 	*scale(mat4(1.0f), vec3(0.5f)));
		// ResourceManager::instance().getModel(2)->Draw(lightShader);

		glfwSwapBuffers(w.getWindow());
		glfwPollEvents();

	} 
	while( glfwGetKey(w.getWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(w.getWindow()) == 0 );

	//Window destructor saves the day
	return EXIT_SUCCESS;
}
