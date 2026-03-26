#include "glm/fwd.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "input_system.hpp"
#include "scene.hpp"
#include <iostream>

#define WIDTH 1000
#define HEIGHT 1000

GLFWwindow *setup_screen();

// Sets up all non generic things. All objects, shaders and input actions should
// be inserted here.
// Scene needs to be a pointer or else it would die when this function ends and
// the inputSystem would reference a dead space
std::pair<Scene *, InputSystem> setup_environment(GLFWwindow *window) {
  // Creates the scene which contain all the data
  Scene *scene =
      new Scene("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

  // Inserts objects
  // Link
  scene->addObject({"horizontal"}, "objects/link.obj",
                   glm::vec3(0.0f, 0.0f, -0.15f), glm::vec3(0.0f, 45.0f, 0.0f),
                   glm::vec3(0.15f, 0.15f, 0.15f));
  // Triforce
  scene->addObject({"vertical", "rotate"}, "objects/triforce.obj",
                   glm::vec3(0.0f, 0.65f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3(0.05f, 0.05f, 0.05f));
  // Sword
  scene->addObject({"horizontal", "scale"}, "objects/sword.obj",
                   glm::vec3(0.11f, 0.02f, -0.55f),
                   glm::vec3(-90.0f, 45.0f, 45.0f),
                   glm::vec3(0.05f, 0.05f, 0.05f));

  // Shield
  scene->addObject({"horizontal"}, "objects/shield.obj",
                   glm::vec3(-0.1f, 0.02f, -0.55f),
                   glm::vec3(0.0f, 45.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f));

  // Rupee (zelda money)
  scene->addObject({"vertical", "horizontal", "rotate"}, "objects/rupee.obj",
                   glm::vec3(-0.5f, 0.15f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3(0.2f, 0.2f, 0.2f));
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Creates the input system and inserts actions and their related keys
  InputSystem inputSystem(scene, window);

  // Vertical translate
  inputSystem.registerKeyAction(GLFW_KEY_W, [](Scene *scene, float delta_time) {
    scene->applyToObjects("vertical", [delta_time](SceneObject *obj) {
      obj->translate(glm::vec3(0, 1 * delta_time, 0));
    });
  });
  inputSystem.registerKeyAction(GLFW_KEY_S, [](Scene *scene, float delta_time) {
    scene->applyToObjects("vertical", [delta_time](SceneObject *obj) {
      obj->translate(glm::vec3(0, -1 * delta_time, 0));
    });
  });
  // Horizontal translate
  inputSystem.registerKeyAction(GLFW_KEY_A, [](Scene *scene, float delta_time) {
    scene->applyToObjects("horizontal", [delta_time](SceneObject *obj) {
      obj->translate(glm::vec3(-1 * delta_time, 0, 0));
    });
  });
  inputSystem.registerKeyAction(GLFW_KEY_D, [](Scene *scene, float delta_time) {
    scene->applyToObjects("horizontal", [delta_time](SceneObject *obj) {
      obj->translate(glm::vec3(1 * delta_time, 0, 0));
    });
  });
  // Rotate on Y axis
  inputSystem.registerKeyAction(GLFW_KEY_Q, [](Scene *scene, float delta_time) {
    scene->applyToObjects("rotate", [delta_time](SceneObject *obj) {
      obj->rotate(glm::vec3(0, 60 * delta_time, 0));
    });
  });
  inputSystem.registerKeyAction(GLFW_KEY_E, [](Scene *scene, float delta_time) {
    scene->applyToObjects("rotate", [delta_time](SceneObject *obj) {
      obj->rotate(glm::vec3(0, -60 * delta_time, 0));
    });
  });
  // Scale
  inputSystem.registerKeyAction(GLFW_KEY_H, [](Scene *scene, float delta_time) {
    scene->applyToObjects("scale", [delta_time](SceneObject *obj) {
      obj->scaleUp(
          glm::vec3(0.5f * delta_time, 0.5f * delta_time, 0.5f * delta_time));
    });
  });
  inputSystem.registerKeyAction(GLFW_KEY_J, [](Scene *scene, float delta_time) {
    scene->applyToObjects("scale", [delta_time](SceneObject *obj) {
      obj->scaleUp(glm::vec3(-0.5f * delta_time, -0.5f * delta_time,
                             -0.5f * delta_time));
    });
  });

  // Toggle mesh fill
  inputSystem.registerKeyAction(
      GLFW_KEY_P, [](Scene *scene, float delta_time) { scene->ToggleFill(); });

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
      glfwCreateWindow(WIDTH, HEIGHT, "CompGrafica - OpenGL", nullptr, nullptr);
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
  glViewport(0, 0, WIDTH, HEIGHT);
  glEnable(GL_DEPTH_TEST);
  return window;
}