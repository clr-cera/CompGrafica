#include "input_system.hpp"

// Just applies the function when a key is pressed
void InputSystem::update() {
  float current_time = glfwGetTime();
  float delta_time = current_time - last_frame_time;
  last_frame_time = current_time;

  for (const auto &pair : key_actions) {
    if (glfwGetKey(window, pair.first) == GLFW_PRESS) {
      pair.second(scene, delta_time);
    }
  }
}