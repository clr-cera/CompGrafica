#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP
#include "mesh.hpp"
#include "shader.hpp"
#include <glm/glm.hpp>
#include <string>

class SceneObject {
public:
  SceneObject() {}
  SceneObject(std::string path);

  Mesh *mesh;

  const void draw(Shader shader);

  glm::mat4 transMatrix();

  void rotate(glm::vec3 rot);
  void scaleUp(glm::vec3 sca);
  void translate(glm::vec3 tra);

  void setRotation(glm::vec3 rot);
  void setScale(glm::vec3 sca);
  void setPosition(glm::vec3 tra);

  glm::vec3 getRotation();
  glm::vec3 getScale();
  glm::vec3 getPosition();

private:
  glm::mat4 cachedTransMatrix;
  bool transMatrixNeedsUpdate = true;

  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

  const void bind(Shader shader);
  const void unbind();
};

#endif // SCENE_OBJECT_HPP