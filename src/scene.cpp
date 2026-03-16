#include "scene.hpp"
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