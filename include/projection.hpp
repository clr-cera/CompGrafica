#ifndef PROJECTION_HPP
#define PROJECTION_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Perspective projection
class Projection {
public:
  Projection(float fov = 45.0f, float aspect_ratio = 1.0f,
             float near_plane = 0.1f, float far_plane = 100.0f)
      : fov(fov), aspect_ratio(aspect_ratio), near_plane(near_plane),
        far_plane(far_plane) {};

  glm::mat4 getProjectionMatrix();

  void zoom(float zoom_delta);

private:
  float fov;
  float aspect_ratio;
  float near_plane;
  float far_plane;
  glm::mat4 cachedProjectionMatrix;
  bool projectionMatrixNeedsUpdate = true;
};
#endif // PROJECTION_HPP