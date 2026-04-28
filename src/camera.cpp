#include <camera.hpp>
#include <iostream>

glm::mat4 Camera::GetViewMatrix() {
  if (viewMatrixNeedsUpdate) {
    cachedViewMatrix = glm::lookAt(position, position + front, up);
    viewMatrixNeedsUpdate = false;
  }
  return cachedViewMatrix;
}

void Camera::updateCameraVectors() {
  viewMatrixNeedsUpdate = true;
  // calculate the new Front vector
  glm::vec3 calculatedFront;
  calculatedFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  calculatedFront.y = sin(glm::radians(pitch));
  calculatedFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(calculatedFront);
  right = glm::normalize(glm::cross(front, world_up));
  up = glm::normalize(glm::cross(right, front));
}

void Camera::translate(Camera_Movement direction, float value) {
  // I want to change this function to only translate on x and z.

  viewMatrixNeedsUpdate = true;
  if (direction == FORWARD) {
    position.x += front.x * value;
    position.z += front.z * value;
  }
  if (direction == BACKWARD) {
    position.x -= front.x * value;
    position.z -= front.z * value;
  }
  if (direction == LEFT) {
    position.x -= right.x * value;
    position.z -= right.z * value;
  }
  if (direction == RIGHT) {
    position.x += right.x * value;
    position.z += right.z * value;
  }
}

void Camera::rotate(float yaw_delta, float pitch_delta) {
  viewMatrixNeedsUpdate = true;
  yaw += yaw_delta;
  pitch += pitch_delta;

  // Make sure that the player cannot do a backflip
  if (pitch > 89.0f) {
    pitch = 89.0f;
  }
  if (pitch < -89.0f) {
    pitch = -89.0f;
  }

  updateCameraVectors();
}

void Camera::updatePosition(float delta_time) {
  translate(FORWARD, this->velocity.x * delta_time * 0.5f);
  translate(LEFT, this->velocity.z * delta_time * 0.5f);
  if (std::abs(this->velocity.y) > 0.001f) {
    this->position.y += this->velocity.y * delta_time * 0.5f;
    this->viewMatrixNeedsUpdate = true;
  }
  std::cout << "Camera Position: " << position.x << ", " << position.y << ", "
            << position.z << std::endl;
}