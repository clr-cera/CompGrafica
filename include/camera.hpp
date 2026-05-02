#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
public:
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f,
         float pitch = 0.0f, float zoom = 45.0f) {
    this->position = position;
    this->world_up = up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->right = glm::vec3(1.0f, 0.0f, 0.0f);
    this->up = up;
    this->cachedViewMatrix = glm::mat4(1.0f);
    this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    updateCameraVectors();
  }

  // translate the camera in a given direction
  void translate(Camera_Movement direction, float value);
  // rotate the camera by changing its yaw and pitch
  void rotate(float yaw_delta, float pitch_delta);
  // set the camera speed
  void setSpeed(glm::vec3 speed) { velocity = speed; }
  // increment the camera speed
  void addSpeed(glm::vec3 speed) { velocity += speed; }
  // uses the camera velocity to update its position
  void updatePosition(float delta_time);
  // Getters and setters for position and velocity
  glm::vec3 getPosition() { return position; }
  glm::vec3 getVelocity() { return velocity; }
  void setPosition(glm::vec3 pos) {
    position = pos;
    viewMatrixNeedsUpdate = true;
  }

  // Returns the view matrix of this camera, it caches if there is no change
  glm::mat4 GetViewMatrix();

private:
  // Camera attributes
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 world_up;
  glm::mat4 cachedViewMatrix;
  glm::vec3 velocity;
  bool viewMatrixNeedsUpdate = true;

  float yaw;
  float pitch;

  void updateCameraVectors();
};

#endif // CAMERA_HPP