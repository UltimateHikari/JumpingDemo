#include "controls.hpp"

#include <iostream>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

float norm(float a){
	return fmod(a, 6.28);
}

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

void FreeCamera :: computeMatrices(float deltaTime){
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

	//std::cerr << to_string(direction) << std::endl;
	//std::cerr << horizontalAngle << " " << verticalAngle << std::endl;
}

void FreeCamera :: setAngles(float horizontal, float vertical){
	horizontalAngle = norm(horizontal);
	verticalAngle = norm(vertical);
}

void FreeCamera :: setPosition(vec3 position_){
	position = position_;
}


TrackingCamera::TrackingCamera() :
	objectPosition(vec3(0,0,0)),
	horizontalAngle(0.0f),
	verticalAngle(0.5f),
	FoV(60.0f),
	distance(10.0f)
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
	horizontalAngle = norm(horizontal + 3.14);
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
		(-1.0f)*cos(verticalAngle) * sin(horizontalAngle)
	)*distance;
	// Right vector - const `cause no roll
	vec3 right = vec3(
		cos(horizontalAngle - 3.14f/2.0f), 
		0,
		(-1.0f)*sin(horizontalAngle - 3.14f/2.0f)
	);
	up = glm::cross(right, direction);
	//std::cerr << to_string(direction) << std::endl;
	//std::cerr << horizontalAngle << " " << verticalAngle << std::endl;
}

void CameraCycler :: switchToNextController()
{
	handlers[index].second->disable();
	index++;
	if(index == handlers.size()) index = 0;
	handlers[index].second->enable();
	handlers[index].first(); //activate that camera in-game

}
void CameraCycler :: registerCameraController(
        std::function<void() > activateCamera,
         ControllerInterface* cameraHandler)
{
	handlers.push_back(std::make_pair(activateCamera, cameraHandler));
}

void Player :: update(Window& window, float deltaTime){
	if(!enabled) return;
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
	vec3 direction = vec3(
		cos(horizontalAngle),
		0.0f,
		(-1.0f)*sin(horizontalAngle)
	);
	vec3 velocity = vec3(0.0);
	vec3 right = cross(direction, vec3(0.0,1.0,0.0));
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

	isInAir = (entity->getPhysical().getPosition().y > 0.0 ? true : false);
	if(!isInAir) jumps[jumpType]->reset();
	jumps[jumpType]->refresh(&(entity->getPhysical()), deltaTime);
	if(hadJumped){
		std::cerr << jumpType << " ";
		if(!isInAir){
			std::cerr << "jOmp\n";
			velocity += vec3(0.0,1.0,0.0) * deltaTime * speed;
		}else{
			velocity += jumps[jumpType]->jump(&(entity->getPhysical()))*deltaTime*speed;
		}
		hadJumped = false;
	}
	//std::cerr << to_string(velocity) << std::endl;
	entity->getPhysical().doMove(velocity);
	camera->setPosition(entity->getPhysical().getPosition()); //delayed move
}

void Player :: receiveCallback(int id){
	if(!enabled) return;
	if(static_cast<Callbacks>(id) == Callbacks::Jump)
		hadJumped = true;
	if(static_cast<Callbacks>(id) == Callbacks::NextJump)
		jumpType = (jumpType + 1) % JUMP_TYPES_COUNT;
}

// void Spectator :: update(Window& window, float deltaTime){

// }

glm::vec3 DoubleJump :: jump(PhysicalObject* physical){
	if(jumpChargesLeft > 0){
		std::cerr << "jOOOmp\n";
		jumpChargesLeft--;
		return vec3(0.0,1.0,0.0);
	}
	return vec3(0.0f);
}

glm::vec3 LiftJump :: jump(PhysicalObject* physical){
	if(hadJumped){
		std::cerr << "not bRrr\n";
		physical->enableGravitation();
		hadJumped = false;
	}else{
		if(jumpChargesLeft > 0){
			std::cerr << "bRrrr\n";
			physical->disableGravitation();
			jumpChargesLeft--;
			hadJumped = true;
			return vec3(0.0,1.5,0.0);
		}
	}
	return vec3(0.0f);
}

glm::vec3 GlideJump :: jump(PhysicalObject* physical){
	if(hadJumped){
		std::cerr << "not whOOsh\n";
		physical->enableGravitation();
		hadJumped = false;
	}else{
		if(jumpChargesLeft > 0){
			std::cerr << "whOOsh\n";
			physical->softenGravitation();
			jumpChargesLeft--;
			hadJumped = true;
			return vec3(0.0,1.0,0.0);
		}
	}
	return vec3(0.0f);
}

void DoubleJump :: reset(){
	jumpChargesLeft = defaultJumpCharges;
}

void LiftJump :: reset(){
	jumpChargesLeft = defaultJumpCharges;
	liftActiveTime = 0.0f;
	hadJumped = false;
}
void GlideJump :: reset(){
	jumpChargesLeft = defaultJumpCharges;
	glideActiveTime = 0.0f;
	hadJumped= false;
}

void LiftJump :: refresh(PhysicalObject* physical, float deltaTime){
	//std::cerr << liftActiveTime << "\n";
	if(hadJumped){
		liftActiveTime += deltaTime;
		if(liftActiveTime > maxLiftTime){
			physical->enableGravitation();
			jumpChargesLeft = 0;
		}
	}
}

void GlideJump :: refresh(PhysicalObject* physical, float deltaTime){
	if(hadJumped){
		glideActiveTime += deltaTime;
		if(glideActiveTime > maxGlideTime){
			physical->enableGravitation();
			jumpChargesLeft = 0;
		}
	}
}

void Roamer :: update(Window& window, float deltaTime){
	if(accumulator < volatility){
		accumulator += deltaTime*6.0;
	}else{
		srand(time(NULL));
		float angle = ((float)rand() / RAND_MAX - 0.5)*6.28;
		//std::cerr << angle << std::endl;
		velocity = vec3(
		cos(angle),
		0.0f,
		(-1.0f)*sin(angle)
		)*speed*deltaTime;
		accumulator = 0.0f;
	}
	entity->getPhysical().doMove(velocity);
}