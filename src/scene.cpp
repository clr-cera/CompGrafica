#include "scene.hpp"
#include "mesh.hpp"
#include "scene_object.hpp"
#include "shader.hpp"
#include <vector>

// Creates a scene with the given shader
Scene::Scene(std::string vertexShaderPath, std::string fragmentShaderPath)
    : shader(vertexShaderPath, fragmentShaderPath) {}

// Clears the screen and draws all objects
void Scene::Render() {
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  shader.use();
  for (auto &scene_object : objects) {
    scene_object->draw(shader);
  }
}

// Adds an object to the scene
void Scene::addObject(std::vector<std::string> components, std::string path) {
  SceneObject *scene_object = new SceneObject(path);
  objects.push_back(scene_object);
  for (const auto &component : components) {
    component_map.emplace(component, scene_object);
  }
}

// Adds an object to the scene with the given transformation
void Scene::addObject(std::vector<std::string> components, std::string path,
                      glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
  SceneObject *scene_object = new SceneObject(path);
  scene_object->setPosition(position);
  scene_object->setRotation(rotation);
  scene_object->setScale(scale);

  objects.push_back(scene_object);
  for (const auto &component : components) {
    component_map.emplace(component, scene_object);
  }
}

// Applies a function to all objects that have the given component
void Scene::applyToObjects(std::string component,
                           std::function<void(SceneObject *)> func) {
  auto range = component_map.equal_range(component);
  for (auto it = range.first; it != range.second; ++it) {
    func(it->second);
  }
}