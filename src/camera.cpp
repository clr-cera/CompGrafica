#include <camera.hpp>

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
  viewMatrixNeedsUpdate = true;
  if (direction == FORWARD) {
    position += front * value;
  }
  if (direction == BACKWARD) {
    position -= front * value;
  }
  if (direction == LEFT) {
    position -= right * value;
  }
  if (direction == RIGHT) {
    position += right * value;
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