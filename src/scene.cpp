#include "scene.hpp"
#include "mesh.hpp"
#include "scene_object.hpp"
#include "shader.hpp"
#include <vector>

Scene::Scene(std::string vertexShaderPath, std::string fragmentShaderPath,
             std::vector<std::string> objectPaths,
             std::vector<std::string> objectNames)
    : shader(vertexShaderPath, fragmentShaderPath) {

  if (objectPaths.size() != objectNames.size()) {
    throw std::runtime_error("Object paths and names must have the same size.");
  }
  for (size_t i = 0; i < objectPaths.size(); ++i) {
    objects.emplace(objectNames[i], SceneObject(objectPaths[i]));
  }
}

void Scene::Render() {
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  shader.use();
  for (auto &pair : objects) {
    pair.second.draw(shader);
  }
}