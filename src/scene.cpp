#include "scene.hpp"
#include "mesh.hpp"
#include "scene_object.hpp"
#include "shader.hpp"
#include <vector>

Scene::Scene(std::string vertexShaderPath, std::string fragmentShaderPath,
             std::vector<std::string> objectPaths)
    : shader(vertexShaderPath, fragmentShaderPath) {
  for (const auto &path : objectPaths) {
    objects.push_back(SceneObject(path));
  }
}

void Scene::Render() {
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  objects[0].rotate(glm::vec3(0.0f, 0.0f, 1.0f));
  objects[1].rotate(glm::vec3(0.0f, 0.0f, -1.0f));

  shader.use();
  for (auto &object : objects) {
    object.draw(shader);
  }
}