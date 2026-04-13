#include "input_system.hpp"
#include "GLFW/glfw3.h"

// Just applies the function when a key is pressed
// delta_time is passed so that functions called can tolerate frame rate changes
void InputSystem::update() {
  float current_time = glfwGetTime();
  float delta_time = current_time - last_frame_time;
  last_frame_time = current_time;

  double current_mouse_x, current_mouse_y;
  glfwGetCursorPos(window, &current_mouse_x, &current_mouse_y);
  if (first_mouse) {
    last_mouse_x = current_mouse_x;
    last_mouse_y = current_mouse_y;
    first_mouse = false;
  }
  double dx = current_mouse_x - last_mouse_x;
  double dy = current_mouse_y - last_mouse_y;
  last_mouse_x = current_mouse_x;
  last_mouse_y = current_mouse_y;

  for (const auto &pair : key_actions) {
    if (glfwGetKey(window, pair.first) == GLFW_PRESS) {
      pair.second(scene, delta_time);
    }
  }
  for (const auto &action : mouse_actions) {
    action(scene, delta_time, dx, dy);
  }
}