#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP
#include "mesh.hpp"
#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

enum class LightZone { Indoor, Outdoor, All };

// SceneObject represents an instance of a mesh,
// it has its own transformation
class SceneObject {
public:
  SceneObject() {}
  SceneObject(std::string path, std::string texture_path);
  SceneObject(std::string path, std::string texture_path, glm::vec3 lightColor);

  Mesh *mesh;

  const void draw(Shader shader);

  glm::mat4 transMatrix();

  // Incremental transformations
  void rotate(glm::vec3 rot);
  void scaleUp(glm::vec3 sca);
  void translate(glm::vec3 tra);
  void accelerate(glm::vec3 acc);

  // Directly set transformations
  void setRotation(glm::vec3 rot);
  void setScale(glm::vec3 sca);
  void setPosition(glm::vec3 tra);
  void setVelocity(glm::vec3 vel);

  // Compute position based on velocity
  void updatePosition(float delta_time);

  glm::vec3 getRotation();
  glm::vec3 getScale();
  glm::vec3 getPosition();
  glm::vec3 getVelocity();

  // Below fields and methods are used for lighting and are only used if the
  // object is a light source
  struct LightSource {
    glm::vec3 centroid;
    glm::vec3 color;
  };
  LightSource lightSource;
  LightZone zone = LightZone::All;
  void setZone(LightZone z) { zone = z; }
  bool flipNormals = false;
  void setFlipNormals(bool flip) { flipNormals = flip; }
  bool lightsEnabled = true;
  void toggleLight() {
    if (last_light_toggle_time + 0.2f > glfwGetTime()) {
      return;
    }
    last_light_toggle_time = glfwGetTime();
    lightsEnabled = !lightsEnabled;
  }
  // These are fields that are used to specify how to light affects each object
  float materialDiffuse = 1.0f;
  float materialSpecular = 0.5f;
  void setMaterialDiffuse(float d) { materialDiffuse = d; }
  void setMaterialSpecular(float s) { materialSpecular = s; }
  float getMaterialDiffuse() const { return materialDiffuse; }
  float getMaterialSpecular() const { return materialSpecular; }

private:
  // Cache the matrix if there is no transformation change
  glm::mat4 cachedTransMatrix;
  glm::mat3 cachedNormalMatrix;
  bool transMatrixNeedsUpdate = true;

  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

  float last_light_toggle_time = 0.0f;

  const void bind(Shader shader);
  const void unbind();
};

#endif // SCENE_OBJECT_HPP