#ifndef INPUT_SYSTEM_HPP
#define INPUT_SYSTEM_HPP
#define GLFW_INCLUDE_NONE

#include "GLFW/glfw3.h"
#include "scene.hpp"
// Handles all input and have access to the scene data. It is generalized enough
// to deal with with objects and on the future camera and light sources.
class InputSystem {
public:
  Scene *scene;
  GLFWwindow *window;
  float last_frame_time;
  std::map<int, std::function<void(Scene *, float delta_time)>> key_actions;

  InputSystem(Scene *scene, GLFWwindow *window)
      : scene(scene), window(window), last_frame_time(0.0f) {}
  void update();

  void
  registerKeyAction(int key,
                    std::function<void(Scene *, float delta_time)> action) {
    key_actions[key] = action;
  }
};

#endif // INPUT_SYSTEM_HPP