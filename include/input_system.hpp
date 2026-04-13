#ifndef INPUT_SYSTEM_HPP
#define INPUT_SYSTEM_HPP
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "scene.hpp"
#include <map>
#include <vector>
// Handles all input and have access to the scene data. It is generalized enough
// to deal with with objects and on the future camera and light sources.
class InputSystem {
public:
  Scene *scene;
  GLFWwindow *window;
  float last_frame_time;
  float last_mouse_x, last_mouse_y;
  bool first_mouse = true;
  std::map<int, std::function<void(Scene *, float delta_time)>> key_actions;
  std::vector<
      std::function<void(Scene *, float delta_time, double dx, double dy)>>
      mouse_actions;

  InputSystem(Scene *scene, GLFWwindow *window)
      : scene(scene), window(window), last_frame_time(0.0f), last_mouse_x(0.0),
        last_mouse_y(0.0) {
    glfwSetWindowUserPointer(window, scene);
  }
  void update();

  void
  registerKeyAction(int key,
                    std::function<void(Scene *, float delta_time)> action) {
    key_actions[key] = action;
  }
  void registerMouseAction(
      std::function<void(Scene *, float delta_time, double dx, double dy)>
          action) {
    mouse_actions.push_back(action);
  }
  void registerMouseScroolCallback(GLFWscrollfun action) {
    glfwSetScrollCallback(window, action);
  }
};

#endif // INPUT_SYSTEM_HPP