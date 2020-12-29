#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "rmanager.hpp"
#include "game.hpp"

using namespace glm;

int main( void ){
	Window w(1024,768);
	ResourceManager manager;
	Game game(w);

	do{
		//game.use_events();
		game.update();
		game.render();
		glfwSwapBuffers(w.getWindow());
		glfwPollEvents();

	} 
	while( glfwGetKey(w.getWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(w.getWindow()) == 0 );

	//Window destructor saves the day
	return EXIT_SUCCESS;
}
