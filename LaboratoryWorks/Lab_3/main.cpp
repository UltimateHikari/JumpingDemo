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

#include <glm/gtx/string_cast.hpp>


int main( void ){
	Window w(1024,768);
	//glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

	Shader shader(
		"../shaders/VShader.vertexshader", 
		"../shaders/FShader.fragmentshader"
		);

	Model model(
		"../resources/checkedCube.obj"
		);
	Model scene(
		"../resources/scene.obj"
		);

	//well, depth-test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Spawn some cameras
	FreeCamera camera;
	glfwSetCursorPos(w.getWindow(), 1024/2, 768/2); // TODO parameters

	do{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		camera.computeMatricesFromInputs(w.getWindow());
		shader.setMat4("projection", camera.getProjectionMatrix());
		shader.setMat4("view", camera.getViewMatrix());
		shader.setMat4("model", translate(mat4(1.0f), vec3(0.0f,2.0f,0.0f))); // E, no translation

		shader.setVec3("light.position", vec3(4.0f,4.0f,2.0f));
		shader.setVec3("light.ambient", vec3(0.1f,0.1f,0.1f));
		shader.setVec3("light.diffuse", vec3(0.6f,0.6f,0.6f));
		shader.setVec3("light.specular", vec3(1.0f,1.0f,1.0f));

		shader.setFloat("light.constant", 1.0f);
		shader.setFloat("light.lin", 0.09f);
		shader.setFloat("light.quad", 0.0032f);

		shader.setVec3("material.specular", vec3(0.5f,0.5f,0.5f));
		shader.setFloat("material.shininess", 32.0f);

		shader.setVec3("viewPosition", camera.getPosition());

		model.Draw(shader);

		shader.setMat4("model", scale(mat4(1.0f), vec3(40.0f,40.0f,40.0f)));

		scene.Draw(shader);

		glfwSwapBuffers(w.getWindow());
		glfwPollEvents();

	} 
	while( glfwGetKey(w.getWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(w.getWindow()) == 0 );

	//Window destructor saves the day
	return EXIT_SUCCESS;
}
