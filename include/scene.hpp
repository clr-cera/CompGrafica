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

#include "lighting.hpp"

// Scene contains all the data of the program, including objects and shaders.
// It also contains a component map, that allow to interact with objects that
// have the same component.
class Scene {
private:
  // Initialization order gambiarra
  Shader shader;
  Shader lightSourceShader;

public:
  Scene(std::string vertexShaderPath, std::string fragmentShaderPath, std::string lightSourceFragShaderPath,
        float aspect_ratio, glm::vec3 indoorBoundsCenter = glm::vec3(0.0f), float indoorBoundsRadius = 0.0f);

  void addObject(std::vector<std::string> components, std::string path,
                 std::string texture_path);
  void addObject(std::vector<std::string> components, std::string path,
                 std::string texture_path, glm::vec3 position,
                 glm::vec3 rotation, glm::vec3 scale,
                 glm::vec3 velocity = glm::vec3(0.0f));
  void addLightObject(std::vector<std::string> components, std::string path,
                      std::string texture_path, glm::vec3 position,
                      glm::vec3 rotation, glm::vec3 scale,
                      glm::vec3 lightColor, LightZone zone = LightZone::All);

  void applyToObjects(std::string component,
                      std::function<void(SceneObject *)>);

  void register_continuous_function(
      std::string component,
      std::function<void(std::vector<SceneObject *>, float)>);

  void register_system(std::function<void(Scene *, float)> system);

  void Render();
  // Runs all systems and render functions, it is called every frame
  void RunSystems();
  void ToggleFill();
  void setIndoorBounds(glm::vec3 center, float radius);
  bool isCameraIndoor() const;

  Camera camera;
  Projection projection;
  Lighting lighting;

private:
  // Maps components to multiple objects
  std::unordered_multimap<std::string, SceneObject *> component_map;
  // Stores objects to be rendered
  std::vector<SceneObject *> objects;
  // Stores functions called on render
  std::unordered_multimap<
      std::string, std::function<void(std::vector<SceneObject *>, float)>>
      render_functions;
  // Stores Systems to act on the scene, it is a more general version of render
  // functions.
  std::vector<std::function<void(Scene *, float)>> systems;

  void update_light_sources();

  glm::vec3 indoorCenter = glm::vec3(0.0f);
  float indoorRadius = 0.0f;

  float last_frame_time = 0.0f;
  // Is used on ToggleFill to change polygon behavior
  struct FillToggle {
    bool should_fill = true;
    float last_toggle_time = 0.0f;
  } fill_toggle;
};
#endif // SCENE_HPP