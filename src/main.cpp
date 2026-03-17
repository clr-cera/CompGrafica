#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <iostream>
#include "obj_parser.h"
#include "mesh.hpp"
#include "shader.hpp"
GLFWwindow *setup();

void verify_mesh_doesnt_crash() {
    // Initialize GLAD

    Mesh mesh("objects/sample.obj");
    mesh.bind();
    mesh.unbind();
}

void verify_mesh_works_kinda(Shader shader) {

    Mesh mesh("objects/sample.obj");
    mesh.bind();
    shader.use();
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
    mesh.unbind();
}

int main() {
  GLFWwindow *window = setup();


  if (!window) {
    return -1;
  }
    Shader shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
  // Main render loop
  while (!glfwWindowShouldClose(window)) {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Set background color (dark blue)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

      verify_mesh_works_kinda(shader);
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