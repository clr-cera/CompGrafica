#include "camera.hpp"
#include "glm/fwd.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "boids.hpp"
#include "glad/glad.h"
#include "input_system.hpp"
#include "scene.hpp"
#include <iostream>

#define WIDTH 1920.f
#define HEIGHT 1080.f

GLFWwindow *setup_screen();

// Sets up all non generic things. All objects, shaders and input actions should
// be inserted here.
// Scene needs to be a pointer or else it would die when this function ends and
// the inputSystem would reference a dead space
std::pair<Scene *, InputSystem> setup_environment(GLFWwindow *window) {
  // Creates the scene which contain all the data
  Scene *scene = new Scene("shaders/vertex_shader.glsl",
                           "shaders/fragment_shader.glsl", WIDTH / HEIGHT);

  // Inserts objects
  // Link
  // scene->addObject({"horizontal"}, "objects/link.obj", "objects/link.png",
  //                  glm::vec3(0.0f, 0.0f, -0.15f), glm::vec3(0.0f, 45.0f,
  //                  0.0f), glm::vec3(0.15f, 0.15f, 0.15f));
  // // Triforce
  // scene->addObject({"vertical", "rotate"}, "objects/triforce.obj",
  // "objects/triforce.png",
  //                  glm::vec3(0.0f, 0.65f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
  //                  glm::vec3(0.05f, 0.05f, 0.05f));
  // // Sword
  // scene->addObject({"horizontal", "scale"}, "objects/sword.obj",
  // "objects/sword.png",
  //                  glm::vec3(0.11f, 0.02f, -0.55f),
  //                  glm::vec3(-90.0f, 45.0f, 45.0f),
  //                  glm::vec3(0.05f, 0.05f, 0.05f));
  //
  // // Shield
  // scene->addObject({"horizontal"}, "objects/shield.obj",
  // "objects/shield.png",
  //                  glm::vec3(-0.1f, 0.02f, -0.55f),
  //                  glm::vec3(0.0f, 45.0f, 0.0f), glm::vec3(0.5f, 0.5f,
  //                  0.5f));
  //
  // // Rupee (zelda money)
  // scene->addObject({"vertical", "horizontal", "rotate"}, "objects/rupee.obj",
  // "objects/rupee.png",
  //                  glm::vec3(-0.5f, 0.15f, 0.0f), glm::vec3(0.0f, 0.0f,
  //                  0.0f), glm::vec3(0.2f, 0.2f, 0.2f));
  // // Floor
  // scene->addObject({}, "objects/floor.obj", "objects/floor.png",
  // glm::vec3(0.0f, -0.5f, 0.0f),
  //                  glm::vec3(-25.0f, 0.0f, 0.0f),
  //                  glm::vec3(3.0f, 3.0f, 3.0f));
  // // Grass
  // for (int i = -10; i <= 10; i++) {
  //   for (int j = -23; j <= 2; j++) {
  //     float randomX = (static_cast<float>(rand()) / RAND_MAX - 0.5f) *
  //                     10.0f; // Random noise between -5 and 5 degrees
  //     float randomY = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 10.0f;
  //     float randomZ = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 10.0f;
  //
  //     scene->addObject({}, "objects/grass.obj", "objects/grass.png",
  //                      glm::vec3(i * 0.1f, -0.3f + j * 0.1f, j * 0.15f),
  //                      glm::vec3(-95.0f + randomX, randomY, randomZ),
  //                      glm::vec3(1.0f, 1.0f, 1.0f));
  //   }
  // }
  // house
  scene->addObject({"house"}, "objects/sponge_bob_house/house.obj",
                   "objects/sponge_bob_house/home.png",
                   glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3(0.7f, 0.7f, 0.7f));

  // outside floor
  scene->addObject({"floor"}, "objects/sponge_bob_dirt/dirt.obj",
                   "objects/sponge_bob_dirt/dirt.png",
                   glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3(0.7f, 0.7f, 0.7f));
  // sky
  scene->addObject({"sky"}, "objects/sky/sky.obj", "objects/sky/sky.png",
                   glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3(0.5f, 0.5f, 0.5f));

  // jellyfish
  scene->addObject({"jellyfish"}, "objects/jellyfish/jellyfish.obj",
                   "objects/jellyfish/jellyfish1.png",
                   glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3(0.01f, 0.01f, 0.01f));
  // floor inside the house
  scene->addObject({"floor"}, "objects/sponge_bob_house_floor/floor.obj",
                   "objects/sponge_bob_house_floor/floor.png",
                   glm::vec3(-4.7f, 0.06f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3(0.72f, 0.72f, 0.72f));

  // spongebob
  scene->addObject({"spongebob"}, "objects/spongebob/spongebobcore.obj",
                   "objects/spongebob/spongebob.png",
                   glm::vec3(0.0f, 0.06f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3(0.6f, 0.6f, 0.6f));
  scene->addObject({"spongebob"}, "objects/spongebob/spongebobarms.obj",
                   "objects/spongebob/spongebob.png",
                   glm::vec3(0.0f, 0.06f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3(0.6f, 0.6f, 0.6f));
  scene->addObject({"spongebob"}, "objects/spongebob/spongebobhips.obj",
                   "objects/spongebob/spongebob.png",
                   glm::vec3(0.0f, 0.06f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3(0.6f, 0.6f, 0.6f));

  // gary
  scene->addObject({"gary"}, "objects/gary/back.obj", "objects/gary/back.png",
                   glm::vec3(1.0f, 0.05f, 1.5f), glm::vec3(0.0f, 180.0f, 0.0f),
                   glm::vec3(0.001f, 0.001f, 0.001f));
  scene->addObject({"gary"}, "objects/gary/eyes.obj", "objects/gary/eyes.png",
                   glm::vec3(1.0f, 0.05f, 1.5f), glm::vec3(0.0f, 180.0f, 0.0f),
                   glm::vec3(0.001f, 0.001f, 0.001f));
  scene->addObject({"gary"}, "objects/gary/lower.obj", "objects/gary/lower.png",
                   glm::vec3(1.0f, 0.05f, 1.5f), glm::vec3(0.0f, 180.0f, 0.0f),
                   glm::vec3(0.001f, 0.001f, 0.001f));

  // patrick
  scene->addObject({"patrick"}, "objects/patrick/patrick.obj",
                   "objects/patrick/patrick.png", glm::vec3(1.5f, 0.00f, 1.0f),
                   glm::vec3(0.0f, -45.0f, 0.0f),
                   glm::vec3(0.03f, 0.03f, 0.03f));

  // Tv
  scene->addObject(
      {"tv"}, "objects/tv/20349_Old_Television_set_v1_Textured.obj",
      "objects/tv/20349_Old_Television_set.jpg", glm::vec3(-0.45, 0.05, 0.1),
      glm::vec3(-90, 0, -90), glm::vec3(0.03f, 0.03f, 0.03f));

  // boids
  spawn_boids(scene, "objects/fish/12265_Fish_v1_L2.obj",
              "objects/fish/fish.jpg");
  // scene->addObject({"fish"}, "objects/fish/12265_Fish_v1_L2.obj",
  // "objects/fish/fish.jpg",
  //   glm::vec3(-1, 0.8, 0), glm::vec3(-90, 0, 0), glm::vec3(0.05f, 0.05f,
  //   0.05f), glm::vec3(1.0f, 1.5f, 1.0f)
  // );
  // scene->register_continuous_function("fish", [](std::vector<SceneObject *>
  // objs, float delta_time) {
  //   for (auto &obj : objs) {
  //     align_boid_to_velocity(obj);
  //   }
  // });
  // scene->addObject({"bird"}, "objects/bird2/DOVE.OBJ",
  // "objects/bird2/DOVE.JPG"); scene->addObject({"bird1"},
  // "objects/bird/bird.obj", "objects/bird/bird.png");

  scene->register_continuous_function(
      "boid", [](std::vector<SceneObject *> objs, float delta_time) {
        boid_iteration(objs, delta_time);
      });

  scene->register_system([](Scene *scene, float delta_time) {
    Camera *camera = &scene->camera;
    // Simple gravity
    camera->addSpeed(glm::vec3(0.0f, -9.81f * delta_time, 0.0f));
    // Camera limits
    if (camera->getPosition().y < 0.5f) {
      camera->setPosition(
          glm::vec3(camera->getPosition().x, 0.5f, camera->getPosition().z));
      camera->setVelocity(
          glm::vec3(camera->getVelocity().x, 0.0f, camera->getVelocity().z));
    }

    if (-2.15 > camera->getPosition().x || camera->getPosition().x > 2.15f) {
      float x = camera->getPosition().x > 2.15f ? 2.15f : -2.15f;
      camera->setPosition(
          glm::vec3(x, camera->getPosition().y, camera->getPosition().z));
    }

    if (-1.35 > camera->getPosition().z || camera->getPosition().z > 2.67f) {
      float z = camera->getPosition().z > 2.67f ? 2.67f : -1.35f;
      camera->setPosition(
          glm::vec3(camera->getPosition().x, camera->getPosition().y, z));
    }

    camera->updatePosition(delta_time);
    scene->applyToObjects("sky", [camera](SceneObject *obj) {
      obj->setPosition(camera->getPosition());
    });
  });
  // scene->register_continuous_function("has_velocity",
  // [](std::vector<SceneObject *> objs, float delta_time) {
  //   for (auto &obj : objs) {
  //     auto vel = obj->getVelocity();
  //     obj->translate(glm::vec3(vel.x * delta_time, vel.y * delta_time, vel.z
  //     * delta_time));
  //   }
  // });

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Creates the input system and inserts actions and their related keys
  InputSystem inputSystem(scene, window);

  // For now this is ugly, but unfortunately glfw only allows callbacks on
  // scroll
  inputSystem.registerMouseScroolCallback(
      [](GLFWwindow *window, double xoffset, double yoffset) {
        Scene *scene = static_cast<Scene *>(glfwGetWindowUserPointer(window));
        if (!scene) {
          return;
        }
        scene->projection.zoom(yoffset);
      });

  inputSystem.registerMouseAction(
      [](Scene *scene, float delta_time, double dx, double dy) {
        scene->camera.rotate(dx * delta_time * 3, -dy * delta_time * 3);
      });
  // Vertical translate
  inputSystem.registerKeyAction(GLFW_KEY_W, [](Scene *scene, float delta_time) {
    scene->camera.translate(FORWARD, delta_time);
  });
  inputSystem.registerKeyAction(GLFW_KEY_S, [](Scene *scene, float delta_time) {
    scene->camera.translate(BACKWARD, delta_time);
  });
  // Horizontal translate
  inputSystem.registerKeyAction(GLFW_KEY_A, [](Scene *scene, float delta_time) {
    scene->camera.translate(LEFT, delta_time);
  });
  inputSystem.registerKeyAction(GLFW_KEY_D, [](Scene *scene, float delta_time) {
    scene->camera.translate(RIGHT, delta_time);
  });
  // Jump!
  inputSystem.registerKeyAction(GLFW_KEY_SPACE, [](Scene *scene,
                                                   float delta_time) {
    scene->camera.setVelocity(glm::vec3(scene->camera.getVelocity().x, 10.0f,
                                        scene->camera.getVelocity().z));
  });

  // Gary Movement
  inputSystem.registerKeyAction(GLFW_KEY_C, [](Scene *scene, float delta_time) {
    scene->applyToObjects("gary", [](SceneObject *obj) {
      obj->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
      obj->translate(glm::vec3(0.1f, 0.0f, 0.0f));
      if (obj->getPosition().x > 2.0f) {
        obj->setPosition(
            glm::vec3(2.0f, obj->getPosition().y, obj->getPosition().z));
      }
    });
  });
  inputSystem.registerKeyAction(GLFW_KEY_X, [](Scene *scene, float delta_time) {
    scene->applyToObjects("gary", [](SceneObject *obj) {
      obj->setRotation(glm::vec3(0.0f, 180.0f, 0.0f));
      obj->translate(glm::vec3(-0.1f, 0.0f, 0.0f));
      if (obj->getPosition().x < -2.0f) {
        obj->setPosition(
            glm::vec3(-2.0f, obj->getPosition().y, obj->getPosition().z));
      }
    });
  });

  // Jellyfish scale
  inputSystem.registerKeyAction(GLFW_KEY_J, [](Scene *scene, float delta_time) {
    scene->applyToObjects("jellyfish", [delta_time](SceneObject *obj) {
      obj->scaleUp(glm::vec3(0.001f, 0.001f, 0.001f));
    });
  });
  inputSystem.registerKeyAction(GLFW_KEY_K, [](Scene *scene, float delta_time) {
    scene->applyToObjects("jellyfish", [delta_time](SceneObject *obj) {
      obj->scaleUp(glm::vec3(-0.001f, -0.001f, -0.001f));
      if (obj->getScale().x < 0.001f) {
        obj->setScale(glm::vec3(0.001f, 0.001f, 0.001f));
      }
    });
  });

  // BobSponge rotate

  inputSystem.registerKeyAction(GLFW_KEY_Q, [](Scene *scene, float delta_time) {
    scene->applyToObjects("spongebob", [delta_time](SceneObject *obj) {
      obj->rotate(glm::vec3(0.0f, 45.0f * delta_time, 0.0f));
    });
  });
  inputSystem.registerKeyAction(GLFW_KEY_E, [](Scene *scene, float delta_time) {
    scene->applyToObjects("spongebob", [delta_time](SceneObject *obj) {
      obj->rotate(glm::vec3(0.0f, -45.0f * delta_time, 0.0f));
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
    scene->RunSystems();
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

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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