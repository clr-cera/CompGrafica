#ifndef SCENE_HPP
#define SCENE_HPP

#include "glm/fwd.hpp"
#include "scene_object.hpp"
#include "shader.hpp"
#include <functional>
#include <map>
#include <string>
#include <vector>

// Scene contains all the data of the program, including objects and shaders.
// It also contains a component map, that allow to interact with objects that
// have the same component.
class Scene {
public:
  Scene(std::string vertexShaderPath, std::string fragmentShaderPath);

  void addObject(std::vector<std::string> components, std::string path);
  void addObject(std::vector<std::string> components, std::string path,
                 glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

  void applyToObjects(std::string component,
                      std::function<void(SceneObject *)>);
  void Render();
  void ToggleFill();

private:
  Shader shader;
  // Maps components to multiple objects
  std::unordered_multimap<std::string, SceneObject *> component_map;
  // Stores objects to be rendered
  std::vector<SceneObject *> objects;
  // Is used on ToggleFill to change polygon behavior
  bool opengl_should_fill;
  float last_toggle_time;
};
#endif // SCENE_HPP