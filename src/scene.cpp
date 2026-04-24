#include "scene.hpp"
#include "mesh.hpp"
#include "scene_object.hpp"
#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <camera.hpp>
#include <vector>

#include "boids.hpp"
#include "boids.hpp"
#include "boids.hpp"
#include "boids.hpp"
#include "boids.hpp"
#include "boids.hpp"
#include "boids.hpp"
#include "boids.hpp"

// Creates a scene with the given shader
Scene::Scene(std::string vertexShaderPath, std::string fragmentShaderPath,
             float aspect_ratio)
    : shader(vertexShaderPath, fragmentShaderPath), camera(glm::vec3(0.0f, 0.0f, 1.0f)),
      projection(45.0f, aspect_ratio, 0.1f, 100.0f) {}

// Clears the screen and draws all objects
void Scene::Render() {
  glClearColor(0.14f, 0.16f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Render functions
  float current_time = glfwGetTime();
  float delta_time = current_time - last_frame_time;
  for (auto &render_function : render_functions) {
    auto component = render_function.first;
    std::vector<SceneObject*> objs;
    for (auto it = component_map.equal_range(component).first; it != component_map.equal_range(component).second; ++it) {
      objs.push_back(it->second);
    }
    render_function.second(objs, delta_time);
  }

  shader.use();
  shader.setMat4("view", camera.GetViewMatrix());
  shader.setMat4("projection", projection.getProjectionMatrix());
  for (auto &scene_object : objects) {
    scene_object->draw(shader);
  }
  last_frame_time = current_time;
}

// Adds an object to the scene
void Scene::addObject(std::vector<std::string> components, std::string path, std::string texture_path) {
  SceneObject *scene_object = new SceneObject(path, texture_path);
  objects.push_back(scene_object);
  for (const auto &component : components) {
    component_map.emplace(component, scene_object);
  }
}

// Adds an object to the scene with the given transformation
void Scene::addObject(std::vector<std::string> components, std::string path, std::string texture_path,
                      glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 velocity) {
  SceneObject *scene_object = new SceneObject(path, texture_path);
  scene_object->setPosition(position);
  scene_object->setRotation(rotation);
  scene_object->setScale(scale);
  scene_object->setVelocity(velocity);

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

// Registers a function to be run at each render iteration, affecting all object marked with component
// Function will receive the array of SceneObjects
void Scene::register_continuous_function(std::string component, std::function<void(std::vector<SceneObject *>, float)> func) {
  render_functions.emplace(component, func);
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