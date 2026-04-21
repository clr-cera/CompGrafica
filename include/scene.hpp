#ifndef SCENE_HPP
#define SCENE_HPP

#include "camera.hpp"
#include "glm/fwd.hpp"
#include "projection.hpp"
#include "scene_object.hpp"
#include "shader.hpp"
#include <functional>
#include <string>
#include <vector>

// Scene contains all the data of the program, including objects and shaders.
// It also contains a component map, that allow to interact with objects that
// have the same component.
class Scene {
public:
  Scene(std::string vertexShaderPath, std::string fragmentShaderPath,
        float aspect_ratio);

  void addObject(std::vector<std::string> components, std::string path, std::string texture_path);
  void addObject(std::vector<std::string> components, std::string path, std::string texture_path,
                 glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

  void applyToObjects(std::string component,
                      std::function<void(SceneObject *)>);
  void Render();
  void ToggleFill();

  Camera camera;
  Projection projection;

private:
  Shader shader;
  // Maps components to multiple objects
  std::unordered_multimap<std::string, SceneObject *> component_map;
  // Stores objects to be rendered
  std::vector<SceneObject *> objects;
  // Is used on ToggleFill to change polygon behavior
  struct FillToggle {
    bool should_fill = true;
    float last_toggle_time = 0.0f;
  } fill_toggle;
};
#endif // SCENE_HPP