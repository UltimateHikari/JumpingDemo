// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

mat4 FreeCamera :: getViewMatrix() const{
	return lookAt(
		position, position+direction, up);
}

mat4 FreeCamera :: getProjectionMatrix() const{
	return perspective(radians(FoV), 
				4.0f / 3.0f, 0.1f, 100.0f);
}

vec3 FreeCamera :: getPosition() const{
	return position;
}

FreeCamera::FreeCamera() :
	position(vec3(3,3,3)),
	horizontalAngle(3.80f),
	verticalAngle(-0.2f),
	speed(3.0f),
	mouseSpeed(0.005f),
	FoV(60.0f)
	{};

FreeCamera::FreeCamera(
	vec3 position_, 
	float hAngle,
	float vAngle) : FreeCamera{} {
		position = position_;
		horizontalAngle = hAngle;
		verticalAngle = vAngle;
	};



void FreeCamera :: computeMatricesFromInputs(GLFWwindow* window, float deltaTime){
	//int w_height, w_width; //TODO: Implement Window wrapper
	//glfwGetWindowSize(window, w_width, w_height)

	// Mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, 1024/2, 768/2); // TODO parameters
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	direction = vec3(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector - const `cause no roll
	vec3 right = vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	up = glm::cross( right, direction );

	if (move_flags[0])
		position += direction * deltaTime * speed;
	if (move_flags[1])
		position -= direction * deltaTime * speed;
	if (move_flags[2])
		position += right * deltaTime * speed;
	if (move_flags[3])
		position -= right * deltaTime * speed;

	move_flags.reset();

	//glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS

	//std::cerr << to_string(direction) << std::endl;
	//std::cerr << horizontalAngle << " " << verticalAngle << std::endl;
}

void FreeCamera :: onUp(){
	move_flags[0] = true;
}
void FreeCamera :: onDown(){
	move_flags[1] = true;
}
void FreeCamera :: onRight(){
	move_flags[2] = true;
}
void FreeCamera :: onLeft(){
	move_flags[3] = true;
}