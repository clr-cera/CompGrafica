#include "scene.hpp"
#include "mesh.hpp"
#include "scene_object.hpp"
#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <camera.hpp>
#include <iostream>
#include <vector>

// Creates a scene with the given shader
Scene::Scene(std::string vertexShaderPath, std::string fragmentShaderPath,
             std::string lightSourceFragShaderPath, float aspect_ratio,
             glm::vec3 indoorBoundsCenter, float indoorBoundsRadius)
    : shader(vertexShaderPath, fragmentShaderPath),
      lightSourceShader(vertexShaderPath, lightSourceFragShaderPath),
      camera(glm::vec3(0.0f, 0.0f, 2.0f)),
      projection(45.0f, aspect_ratio, 0.1f, 100.0f),
      lighting(0.5, glGetUniformLocation(shader.programID, "ambientLight"),
               glGetUniformLocation(shader.programID, "ambientColor"),
               glGetUniformLocation(shader.programID, "specularStrength"),
               glGetUniformLocation(shader.programID, "diffuseStrength")),
      indoorCenter(indoorBoundsCenter), indoorRadius(indoorBoundsRadius) {}

// Adds an object to the scene
void Scene::addObject(std::vector<std::string> components, std::string path,
                      std::string texture_path) {
  SceneObject *scene_object = new SceneObject(path, texture_path);
  objects.push_back(scene_object);
  for (const auto &component : components) {
    component_map.emplace(component, scene_object);
  }
  component_map.emplace("__default__", scene_object);
}

// Adds an object to the scene with the given transformation
void Scene::addObject(std::vector<std::string> components, std::string path,
                      std::string texture_path, glm::vec3 position,
                      glm::vec3 rotation, glm::vec3 scale, glm::vec3 velocity) {
  SceneObject *scene_object = new SceneObject(path, texture_path);
  scene_object->setPosition(position);
  scene_object->setRotation(rotation);
  scene_object->setScale(scale);
  scene_object->setVelocity(velocity);

  objects.push_back(scene_object);
  for (const auto &component : components) {
    component_map.emplace(component, scene_object);
  }
  component_map.emplace("__default__", scene_object);
}

void Scene::addLightObject(std::vector<std::string> components,
                           std::string path, std::string texture_path,
                           glm::vec3 position, glm::vec3 rotation,
                           glm::vec3 scale, glm::vec3 lightColor,
                           LightZone zone) {
  SceneObject *scene_object = new SceneObject(path, texture_path, lightColor);
  scene_object->setPosition(position);
  scene_object->setRotation(rotation);
  scene_object->setScale(scale);
  scene_object->setZone(zone);

  objects.push_back(scene_object);
  for (const auto &component : components) {
    component_map.emplace(component, scene_object);
  }
  component_map.emplace("__light_source__", scene_object);
}

// Applies a function to all objects that have the given component
void Scene::applyToObjects(std::string component,
                           std::function<void(SceneObject *)> func) {
  auto range = component_map.equal_range(component);
  for (auto it = range.first; it != range.second; ++it) {
    func(it->second);
  }
}

// Registers a function to be run at each render iteration, affecting all object
// marked with component Function will receive the array of SceneObjects
void Scene::register_continuous_function(
    std::string component,
    std::function<void(std::vector<SceneObject *>, float)> func) {
  render_functions.emplace(component, func);
}

void Scene::register_system(std::function<void(Scene *, float)> system) {
  systems.push_back(system);
}

// Clears the screen and draws all objects
void Scene::Render() {
  glClearColor(0.14f, 0.16f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader.use();
  lighting.set_ambient_uniform();
  shader.setVec3("cameraPos", camera.getPosition());
  shader.setMat4("view", camera.GetViewMatrix());
  shader.setMat4("projection", projection.getProjectionMatrix());
  auto default_objs = component_map.equal_range("__default__");
  for (auto it = default_objs.first; it != default_objs.second; ++it) {
    shader.setBool("flipNormals", it->second->flipNormals);
    it->second->draw(shader);
  }
  lightSourceShader.use();
  lightSourceShader.setMat4("view", camera.GetViewMatrix());
  lightSourceShader.setMat4("projection", projection.getProjectionMatrix());
  auto light_objs = component_map.equal_range("__light_source__");
  for (auto it = light_objs.first; it != light_objs.second; ++it) {
    it->second->draw(lightSourceShader);
  }
}

// Run systems and render functions
void Scene::RunSystems() {
  float current_time = glfwGetTime();
  float delta_time = current_time - last_frame_time;
  for (auto &system : systems) {
    system(this, delta_time);
  }

  // Render functions
  for (auto &render_function : render_functions) {
    auto component = render_function.first;
    std::vector<SceneObject *> objs;
    for (auto it = component_map.equal_range(component).first;
         it != component_map.equal_range(component).second; ++it) {
      objs.push_back(it->second);
    }
    render_function.second(objs, delta_time);
  }

  update_light_sources();

  last_frame_time = current_time;
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

// Handles updating light sources positions in the
// shader array
void Scene::update_light_sources() {
  shader.use();
  LightZone currentZone =
      isCameraIndoor() ? LightZone::Indoor : LightZone::Outdoor;
  auto light_sources = component_map.equal_range("__light_source__");
  int i = 0;
  for (auto it = light_sources.first; it != light_sources.second; ++it) {
    if ((it->second->zone != LightZone::All && it->second->zone != currentZone) ||
        !it->second->lightsEnabled)
      continue;
    shader.setVec3("pointLights[" + std::to_string(i) + "].position",
                   it->second->getPosition());
    shader.setVec3("pointLights[" + std::to_string(i++) + "].color",
                   it->second->lightSource.color);
  }
  shader.setInt("currentLightCount", i);
}

void Scene::setIndoorBounds(glm::vec3 center, float radius) {
  indoorCenter = center;
  indoorRadius = radius;
}

bool Scene::isCameraIndoor() const {
  glm::vec3 pos = const_cast<Camera &>(camera).getPosition();
  float distance = glm::distance(pos, indoorCenter);

  return distance <= indoorRadius;
}
