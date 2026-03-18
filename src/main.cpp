#include "glm/fwd.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "input_system.hpp"
#include "scene.hpp"
#include <iostream>

GLFWwindow *setup_screen();

// Sets up all non generic things. All objects, shaders and input actions should
// be inserted here.
// Scene needs to be a pointer or else it would die when this function ends and
// the inputSystem would reference a dead space
std::pair<Scene *, InputSystem> setup_environment(GLFWwindow *window) {
  // Creates the scene which contain all the data
  Scene *scene = new Scene(
      "shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl",
      {"objects/sample.obj", "objects/sample.obj"}, {"left", "right"});
  // Creates the input system and inserts actions and their related keys
  InputSystem inputSystem(scene, window);

  inputSystem.registerKeyAction(GLFW_KEY_W, [](Scene *scene, float delta_time) {
    scene->objects["left"].rotate(glm::vec3(0, 0, 45 * delta_time));
    scene->objects["right"].rotate(glm::vec3(0, 0, -45 * delta_time));
  });

  return {scene, inputSystem};
}

// Initializes window, screen and input system, then enters the main loop
int main() {
  GLFWwindow *window = setup_screen();

  if (!window) {
    return -1;
  }

  auto [scene, inputSystem] = setup_environment(window);

  // Main render loop
  while (!glfwWindowShouldClose(window)) {
    inputSystem.update();
    scene->Render();

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }

  // Clean up
  delete scene;
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

GLFWwindow *setup_screen() {
  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return nullptr;
  }

  // Create a windowed mode window and its OpenGL context
  GLFWwindow *window =
      glfwCreateWindow(800, 600, "CompGrafica - OpenGL", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return nullptr;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return nullptr;
  }
  return window;
}