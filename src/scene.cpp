#include "scene.hpp"
#include "mesh.hpp"
#include "scene_object.hpp"
#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <vector>

// Creates a scene with the given shader
Scene::Scene(std::string vertexShaderPath, std::string fragmentShaderPath)
    : shader(vertexShaderPath, fragmentShaderPath) {}

// Clears the screen and draws all objects
void Scene::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.14f, 0.16f, 0.3f, 1.0f);

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

void Scene::ToggleFill() {
  if (fill_toggle.last_toggle_time + 0.2f > glfwGetTime()) {
    return;
  }
  fill_toggle.last_toggle_time = glfwGetTime();
  fill_toggle.should_fill = !fill_toggle.should_fill;
  if (fill_toggle.should_fill) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
}