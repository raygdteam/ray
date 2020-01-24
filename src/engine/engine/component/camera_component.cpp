#include "pch.h"
#include "camera_component.hpp"
#include <GLM/ext/matrix_transform.inl>
#include "input/input.hpp"

namespace ray::component
{

void CameraComponent::updateViewMatrix()
{
	glm::mat4 rotM = glm::mat4();
	glm::mat4 transM;

	rotM = glm::rotate(rotM, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotM = glm::rotate(rotM, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotM = glm::rotate(rotM, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	transM = glm::translate(glm::mat4(), _position);

	matrices.view = rotM * transM;
}

void CameraComponent::Tick(float delta)
{
	if (ray::Input.keys[KEY_W] || ray::Input.keys[KEY_A] || ray::Input.keys[KEY_S] || ray::Input.keys[KEY_D]) updateViewMatrix();
}

void CameraComponentActor::Tick(float delta)
{
	glm::vec3 camFront;
	camFront.x = -cos(glm::radians(_rotation.x)) * sin(glm::radians(_rotation.y));
	camFront.y = sin(glm::radians(_rotation.x));
	camFront.z = cos(glm::radians(_rotation.x)) * cos(glm::radians(_rotation.y));
	camFront = glm::normalize(camFront);

	float moveSpeed = delta * 1.f;

	if (Input.keys[KEY_W])
		_position += camFront * moveSpeed;
	
	if (Input.keys[KEY_S])
		_position -= camFront * moveSpeed;

	if (Input.keys[KEY_A])
		_position -= glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f))) * moveSpeed;
	
	if (Input.keys[KEY_A])
		_position += glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f))) * moveSpeed;
}
}
