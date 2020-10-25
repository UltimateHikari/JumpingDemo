#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <stdlib.h>
#include <random>
#include <time.h>

#include "../engine/shader.hpp"
#include "../engine/controls.hpp"
#include "../engine/objloader.hpp"
#include "../engine/vboindexer.hpp"
#include "window.hpp"

#include <glm/gtx/string_cast.hpp>


int main( void )
{
	// initialise() 
	Window w(1024,768);
	//glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//shaders
	GLuint programID = LoadShaders( 
		"../shaders/VShader.vertexshader", 
		"../shaders/FShader.fragmentshader"
		);
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	//obj
	std::vector< short unsigned int > indices;
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals; 
	if(!loadAssImp("../src/cube.obj", indices, vertices, uvs, normals)){
		return EXIT_FAILURE;
	}
	//std::cerr << vertices.size() << " " << indices.size() << std::endl;



	srand(time(NULL));
	static GLfloat g_color_buffer_data[12*3*3];
	for(int i = 0; i < 12*3; ++i){
		g_color_buffer_data[3*i] = (float)rand()/RAND_MAX;
		g_color_buffer_data[3*i + 1] = (float)rand()/RAND_MAX;
		g_color_buffer_data[3*i + 2] = (float)rand()/RAND_MAX;
	}


	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


	//well, depth-test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Spawn some cameras
	FreeCamera camera;
	glfwSetCursorPos(w.getWindow(), 1024/2, 768/2); // TODO parameters

	do{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		camera.computeMatricesFromInputs(w.getWindow());
		mat4 projection = camera.getProjectionMatrix();
		mat4 view = camera.getViewMatrix();
		mat4 model = mat4(1.0f); // E, no translation
		mat4 mvp = projection * view * model; //backwards

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0); //Draw finally

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(w.getWindow());
		glfwPollEvents();

	} 
	while( glfwGetKey(w.getWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(w.getWindow()) == 0 );

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	return EXIT_SUCCESS;
}

