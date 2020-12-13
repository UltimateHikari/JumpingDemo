#include "controls.hpp"

#include <iostream>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

float norm(float a){
	return fmod(a, 6.28);
}

// mat4 FreeCamera :: getViewMatrix() const{
// 	return lookAt(
// 		position, position+direction, up);
// }

// mat4 FreeCamera :: getProjectionMatrix() const{
// 	return perspective(radians(FoV), 
// 				4.0f / 3.0f, 0.1f, 100.0f);
// }

// vec3 FreeCamera :: getPosition() const{
// 	return position;
// }

// FreeCamera::FreeCamera() :
// 	position(vec3(3,3,3)),
// 	horizontalAngle(3.80f),
// 	verticalAngle(-0.2f),
// 	speed(3.0f),
// 	mouseSpeed(0.005f),
// 	FoV(60.0f)
// 	{};

// FreeCamera::FreeCamera(
// 	vec3 position_, 
// 	float hAngle,
// 	float vAngle) : FreeCamera{} {
// 		position = position_;
// 		horizontalAngle = hAngle;
// 		verticalAngle = vAngle;
// 	};

// void FreeCamera :: computeMatrices(GLFWwindow* window, float deltaTime){
// 	// Direction : Spherical coordinates to Cartesian coordinates conversion
// 	direction = vec3(
// 		cos(verticalAngle) * sin(horizontalAngle), 
// 		sin(verticalAngle),
// 		cos(verticalAngle) * cos(horizontalAngle)
// 	);
// 	// Right vector - const `cause no roll
// 	vec3 right = vec3(
// 		sin(horizontalAngle - 3.14f/2.0f), 
// 		0,
// 		cos(horizontalAngle - 3.14f/2.0f)
// 	);
	
// 	up = glm::cross( right, direction );

// 	//std::cerr << to_string(direction) << std::endl;
// 	//std::cerr << horizontalAngle << " " << verticalAngle << std::endl;
// }

// void FreeCamera :: setAngles(float horizontal, float vertical){
// 	horizontalAngle = norm(horizontal);
// 	verticalAngle = norm(vertical);
// }

// void FreeCamera :: setPosition(vec3 position_){
// 	position = position_;
// }


TrackingCamera::TrackingCamera() :
	objectPosition(vec3(0,0,0)),
	horizontalAngle(0.2f),
	verticalAngle(0.2f),
	FoV(60.0f),
	distance(6.0f)
	{};

TrackingCamera::TrackingCamera(
	vec3 objectPosition_, 
	float distance_
	) : TrackingCamera{} {
		objectPosition = objectPosition_;
		distance = distance_;
	};

mat4 TrackingCamera :: getViewMatrix() const{
	return lookAt(
		objectPosition + direction, objectPosition, up);
}

mat4 TrackingCamera :: getProjectionMatrix() const{
	return perspective(radians(FoV), 
				4.0f / 3.0f, 0.1f, 100.0f);
}

void TrackingCamera :: setAngles(float horizontal, float vertical){
	horizontalAngle = norm(horizontal);
	verticalAngle = norm(vertical);
}

void TrackingCamera :: setPosition(vec3 position_){
	objectPosition = position_;
}

vec3 TrackingCamera :: getPosition() const{
	return objectPosition + direction;
}

void TrackingCamera :: computeMatrices(float deltaTime){
	direction = vec3(
		cos(verticalAngle) * cos(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * sin(horizontalAngle)
	)*distance;
	// Right vector - const `cause no roll
	vec3 right = vec3(
		cos(horizontalAngle - 3.14f/2.0f), 
		0,
		sin(horizontalAngle - 3.14f/2.0f)
	);
	up = glm::cross( right, direction );
	//std::cerr << to_string(direction) << std::endl;
	//std::cerr << horizontalAngle << " " << verticalAngle << std::endl;
}

void Player :: update(Window& window, float deltaTime){
	float horizontalAngle = entity->getPhysical().getAngle();
	//int w_height, w_width; //TODO: Implement Window wrapper
	//glfwGetWindowSize(window, w_width, w_height)

	// Mouse position
	double xpos, ypos;
	glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
	glfwSetCursorPos(window.getWindow(), 1024/2, 768/2); // TODO parameters
	float horizontalAngleDelta = angleSpeed * float(1024/2 - xpos );
	float verticalAngleDelta   = angleSpeed * float( 768/2 - ypos );

	//sync angles
	camera->setAngles(
		horizontalAngle + horizontalAngleDelta,
		camera->getVerticalAngle() + verticalAngleDelta);
	entity->getPhysical().doTurn(horizontalAngleDelta);

	//move object and notify camera
	vec3 direction = rotateY(vec3(1.0,0.0,0.0), horizontalAngle);
	vec3 velocity = vec3(0.0);
	vec3 right = cross(vec3(0.0,1.0,0.0), direction);
    if(glfwGetKey( window.getWindow(), GLFW_KEY_W ) == GLFW_PRESS){
        velocity += direction * deltaTime * speed;
    }
    if(glfwGetKey( window.getWindow(), GLFW_KEY_S ) == GLFW_PRESS){
        velocity -= direction * deltaTime * speed;
    }
    if(glfwGetKey( window.getWindow(), GLFW_KEY_D ) == GLFW_PRESS){
        velocity += right * deltaTime * speed;
    }
    if(glfwGetKey( window.getWindow(), GLFW_KEY_A ) == GLFW_PRESS){
       velocity -= right * deltaTime * speed;
    }
	entity->getPhysical().doMove(velocity);
	camera->setPosition(entity->getPhysical().getPosition()); //delayed move
}