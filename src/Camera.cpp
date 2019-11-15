#include "Camera.hpp"
#include <iostream>

void Camera::updateVectors(){
	// Get new cameraFront vector
	glm::vec3 newDir;
	newDir.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
	newDir.y = sinf(glm::radians(pitch));
	newDir.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
	cameraFront = glm::normalize(newDir);

	// Get new local up vector
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

	// Check for resized window
	if (camInViewPort != NULL){
		int width, height;
		glfwGetWindowSize(camInViewPort, &width, &height);

		projectionMatrix = glm::perspectiveFov(glm::radians(perspectiveFov), (float)width, (float)height, REDNER_NEAR, RENDER_DISTANCE);
	}
}

Camera::Camera(glm::vec3 pos, glm::vec3 up, float pitch, float yaw) : perspectiveFov(FOV), speed(SPEED), mouseSensitivity(MOUSE_SENSITIVITY), cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)) {
	this->position = pos;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;

	projectionMatrix = glm::perspectiveFov(glm::radians(perspectiveFov), (float)800, (float)600, REDNER_NEAR, RENDER_DISTANCE);

	updateVectors();
}

glm::mat4 Camera::getViewMatrix(){
	return glm::lookAt(position, position + cameraFront, cameraUp);
}

glm::mat4 Camera::getProjectionMatrix(){
	return projectionMatrix;
}

void Camera::move(Camera_Movement dir, float dt){
	float velocity = speed * dt;
	if (dir == FORWARD)
		position += cameraFront * velocity;
	if (dir == BACKWARD)
		position -= cameraFront * velocity;
	if (dir == LEFT)
		position -= cameraRight * velocity;
	if (dir == RIGHT)
		position += cameraRight * velocity;
	if (dir == UP)
		position += worldUp * velocity;
	if (dir == DOWN)
		position -= worldUp * velocity;
}

void Camera::rotate(float xoffset, float yoffset, GLboolean constrained){
	yaw += xoffset * mouseSensitivity;
	pitch += yoffset * mouseSensitivity;

	// Limit the camera angle (pros to stop gimble limmits)
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}

void Camera::zoom(float zoom){
}

void Camera::setFov(float fov){
}

void Camera::setPerspective(){
}

void Camera::setPerspective(float fov){
}

void Camera::setOrthographic(){
}

void Camera::setViewPort(GLFWwindow * context){
	this->camInViewPort = context;
}