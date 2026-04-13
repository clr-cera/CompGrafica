#include <projection.hpp>

glm::mat4 Projection::getProjectionMatrix() {
  if (projectionMatrixNeedsUpdate) {
    cachedProjectionMatrix = glm::perspective(glm::radians(fov), aspect_ratio,
                                              near_plane, far_plane);
    projectionMatrixNeedsUpdate = false;
  }
  return cachedProjectionMatrix;
}
void Projection::zoom(float zoom_delta) {
  fov -= zoom_delta;
  if (fov < 1.0f) {
    fov = 1.0f;
  }
  if (fov > 90.0f) {
    fov = 90.0f;
  }
  projectionMatrixNeedsUpdate = true;
}
