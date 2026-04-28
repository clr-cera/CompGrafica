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

  void addObject(std::vector<std::string> components, std::string path,
                 std::string texture_path);
  void addObject(std::vector<std::string> components, std::string path,
                 std::string texture_path, glm::vec3 position,
                 glm::vec3 rotation, glm::vec3 scale,
                 glm::vec3 velocity = glm::vec3(0.0f));

  void applyToObjects(std::string component,
                      std::function<void(SceneObject *)>);

  void register_continuous_function(
      std::string component,
      std::function<void(std::vector<SceneObject *>, float)>);

  void register_system(std::function<void(Scene *, float)> system);

  void Render();
  void RunSystems();
  void ToggleFill();

  Camera camera;
  Projection projection;

private:
  Shader shader;
  // Maps components to multiple objects
  std::unordered_multimap<std::string, SceneObject *> component_map;
  // Stores objects to be rendered
  std::vector<SceneObject *> objects;
  // Stores functions called on render
  std::unordered_multimap<
      std::string, std::function<void(std::vector<SceneObject *>, float)>>
      render_functions;
  // Stores Systems to act on the scene, it is a more general version of render
  // functions Keeps the time from last frame, updated on render
  std::vector<std::function<void(Scene *, float)>> systems;

  float last_frame_time = 0.0f;
  // Is used on ToggleFill to change polygon behavior
  struct FillToggle {
    bool should_fill = true;
    float last_toggle_time = 0.0f;
  } fill_toggle;
};
#endif // SCENE_HPP