#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP
#include "mesh.hpp"
#include <glm/glm.hpp>
#include <string>

class SceneObject {
public:
  SceneObject(std::string path);

  glm::vec3 rotation;
  glm::vec3 scale;
  glm::vec3 position;
  Mesh mesh;

  void draw();

  glm::mat4 transMatrix();
};

#endif // SCENE_OBJECT_HPP