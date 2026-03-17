#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "scene.hpp"
#include <iostream>
GLFWwindow *setup();

int main() {
  GLFWwindow *window = setup();

  if (!window) {
    return -1;
  }

  Scene scene("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl",
              {"objects/sample.obj", "objects/sample.obj"}, {"left", "right"});

  // Main render loop
  while (!glfwWindowShouldClose(window)) {
    scene.Render();

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }

  // Clean up
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

GLFWwindow *setup() {
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