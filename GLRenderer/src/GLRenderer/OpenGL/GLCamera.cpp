#include <GLRenderer/OpenGL/GLCamera.hpp>
#include <iostream>
#include <cmath> // for exp

namespace GLRenderer {
	GLCamera::GLCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : attributes_() {
		attributes_.position = position;
		attributes_.worldUp = up;
		attributes_.yaw = yaw;
		attributes_.pitch = pitch;
		updateCameraVectors();
	}

	GLCamera::GLCamera(float posX, float posY, float posZ,
		float upX, float upY, float upZ,
		float yaw, float pitch) : attributes_() {
		attributes_.position = glm::vec3(posX, posY, posZ);
		attributes_.worldUp = glm::vec3(upX, upY, upZ);
		attributes_.yaw = yaw;
		attributes_.pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 GLCamera::getViewMatrix() const
	{
		return glm::lookAt(attributes_.position, attributes_.position + attributes_.front, attributes_.up);
	}

	void GLCamera::processKeyboard(Camera_Movement direction, float deltaTime) {
		float velocity = attributes_.movementSpeed * deltaTime;

		if (direction == FORWARD)
			attributes_.targetPosition += attributes_.front * velocity;
		else if (direction == BACKWARD)
			attributes_.targetPosition -= attributes_.front * velocity;
		else if (direction == LEFT)
			attributes_.targetPosition -= attributes_.right * velocity;
		else if (direction == RIGHT)
			attributes_.targetPosition += attributes_.right * velocity;
		else if (direction == UP)
			attributes_.targetPosition += attributes_.up * velocity;
		else if (direction == DOWN)
			attributes_.targetPosition -= attributes_.up * velocity;
	}

	void GLCamera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
		xoffset *= attributes_.mouseSensitivity;
		yoffset *= attributes_.mouseSensitivity;

		attributes_.yaw += xoffset;
		attributes_.pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (attributes_.pitch > 89.0f)
				attributes_.pitch = 89.0f;
			if (attributes_.pitch < -89.0f)
				attributes_.pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	void GLCamera::processMouseScroll(float yoffset) {
		attributes_.zoom -= (float)yoffset;
		if (attributes_.zoom < 1.0f)
			attributes_.zoom = 1.0f;
		if (attributes_.zoom > 45.0f)
			attributes_.zoom = 45.0f;
	}

	void GLCamera::updatePosition(float deltaTime) {
		float movementSmoothFactor = 10.0f * deltaTime;  // Tune this for smoothness
		attributes_.position = lerp(attributes_.position, attributes_.targetPosition, movementSmoothFactor);
		updateCameraVectors();
	}

	void GLCamera::updateCameraVectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(attributes_.yaw)) * cos(glm::radians(attributes_.pitch));
		front.y = sin(glm::radians(attributes_.pitch));
		front.z = sin(glm::radians(attributes_.yaw)) * cos(glm::radians(attributes_.pitch));
		attributes_.front = glm::normalize(front);

		attributes_.right = glm::normalize(glm::cross(attributes_.front, attributes_.worldUp));
		attributes_.up = glm::normalize(glm::cross(attributes_.right, attributes_.front));
	}

	glm::vec3 GLCamera::lerp(const glm::vec3& start, const glm::vec3& end, float t) {
		return start + t * (end - start);
	}

}

