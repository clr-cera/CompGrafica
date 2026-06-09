#include "camera.hpp"
#include "glm/fwd.hpp"
#include <cmath>
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
  Scene *scene = new Scene(
      "shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl",
      "shaders/lightSource_fragment_shader.glsl", WIDTH / HEIGHT,
      glm::vec3(0.0f, 0.5f, 0.0f), // indoor bounds center (house center)
      1.0f);                       // indoor bounds radius

  // FYI: Link is saved in another place to keep code clean

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
  scene->addLightObject(
      {"jellyfish"}, "objects/jellyfish/jellyfish.obj",
      "objects/jellyfish/jellyfish1.png", glm::vec3(0.0f, 0.0f, -5.0f),
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f),
      glm::vec3(0.9, 0.1, 0.9), LightZone::Outdoor);
  scene->addLightObject(
      {"jellyfish"}, "objects/jellyfish/jellyfish.obj",
      "objects/jellyfish/jellyfish1.png", glm::vec3(5.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f),
      glm::vec3(0.9f, 0.1f, 0.9f), LightZone::Outdoor);
  // scene->addLightObject(
  //     {"jellyfish"}, "objects/jellyfish/jellyfish.obj",
  //     "objects/jellyfish/jellyfish1.png", glm::vec3(0.0f, 0.0f, 5.0f),
  //     glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f),
  //     glm::vec3(1.0f, 1.0f, 1.0f));
  // scene->addLightObject(
  //     {"jellyfish"}, "objects/jellyfish/jellyfish.obj",
  //     "objects/jellyfish/jellyfish1.png", glm::vec3(-5.0f, 0.0f, 0.0f),
  //     glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f),
  //     glm::vec3(1.0f, 1.0f, 1.0f));

  // floor inside the house
  scene->addObject(
      {"floor", "inverted_normals"}, "objects/sponge_bob_house_floor/floor.obj",
      "objects/sponge_bob_house_floor/floor.png", glm::vec3(-4.7f, 0.06f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.72f, 0.72f, 0.72f));

  // spongebob
  scene->addObject(
      {"spongebob", "inverted_normals"}, "objects/spongebob/spongebobcore.obj",
      "objects/spongebob/spongebob.png", glm::vec3(0.0f, 0.06f, -0.5f),
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.6f, 0.6f));
  scene->addObject(
      {"spongebob", "inverted_normals"}, "objects/spongebob/spongebobarms.obj",
      "objects/spongebob/spongebob.png", glm::vec3(0.0f, 0.06f, -0.5f),
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.6f, 0.6f));
  scene->addObject(
      {"spongebob", "inverted_normals"}, "objects/spongebob/spongebobhips.obj",
      "objects/spongebob/spongebob.png", glm::vec3(0.0f, 0.06f, -0.5f),
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.6f, 0.6f));

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
      "objects/tv/20349_Old_Television_set.jpg", glm::vec3(-0.45, 0.06, 0.1),
      glm::vec3(-90, 0, -90), glm::vec3(0.03f, 0.03f, 0.03f));

  // Angler Fish
  scene->addLightObject(
      {"anglerfish"}, "objects/anglerfish/body.obj",
      "objects/anglerfish/body.png", glm::vec3(-0.35f, 0.4f, -0.35f),
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(40.0f, 40.0f, 40.0f),
      glm::vec3(1.0f, 1.0f, 0.0f), LightZone::Indoor);
  scene->addLightObject(
      {"anglerfish"}, "objects/anglerfish/eyes.obj",
      "objects/anglerfish/eyes.png", glm::vec3(-0.35f, 0.4f, -0.35f),
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(40.0f, 40.0f, 40.0f),
      glm::vec3(1.0f, 1.0f, 0.0f), LightZone::Indoor);
  scene->addLightObject(
      {"anglerfish"}, "objects/anglerfish/fins.obj",
      "objects/anglerfish/fins.png", glm::vec3(-0.35f, 0.4f, -0.35f),
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(40.0f, 40.0f, 40.0f),
      glm::vec3(1.0f, 1.0f, 0.0f), LightZone::Indoor);

  // small hand lamp
  scene->addLightObject(
      {"small_lamp"}, "objects/smalllamp/smalllamp.obj",
      "objects/smalllamp/smalllamp.png", glm::vec3(0.48f, 0.17f, 0.24f),
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f),
      glm::vec3(0.0f, 1.0f, 1.0f), LightZone::Indoor);

  // Mr krabs
  scene->addObject({"mr_krabs"}, "objects/mr_krabs/mr._krabs.obj",
                   "objects/mr_krabs/Material.png", glm::vec3(0.45, 0.05, 0),
                   glm::vec3(90, 0, 66), glm::vec3(0.0007, 0.0007, 0.0007));

  // boids
  spawn_boids(scene, "objects/fish/12265_Fish_v1_L2.obj",
              "objects/fish/fish.jpg");

  scene->applyToObjects("inverted_normals",
                        [](SceneObject *obj) { obj->setFlipNormals(true); });

  // Per-object material parameters
  scene->applyToObjects("house", [](SceneObject *obj) {
    obj->setMaterialDiffuse(0.9f);
    obj->setMaterialSpecular(0.3f);
  });
  scene->applyToObjects("floor", [](SceneObject *obj) {
    obj->setMaterialDiffuse(0.8f);
    obj->setMaterialSpecular(0.9f);
  });
  scene->applyToObjects("sky", [](SceneObject *obj) {
    obj->setMaterialDiffuse(0.7f);
    obj->setMaterialSpecular(0.1f);
  });
  scene->applyToObjects("spongebob", [](SceneObject *obj) {
    obj->setMaterialDiffuse(0.85f);
    obj->setMaterialSpecular(0.4f);
  });
  scene->applyToObjects("gary", [](SceneObject *obj) {
    obj->setMaterialDiffuse(0.75f);
    obj->setMaterialSpecular(0.5f);
  });
  scene->applyToObjects("patrick", [](SceneObject *obj) {
    obj->setMaterialDiffuse(0.8f);
    obj->setMaterialSpecular(0.35f);
  });
  scene->applyToObjects("tv", [](SceneObject *obj) {
    obj->setMaterialDiffuse(0.7f);
    obj->setMaterialSpecular(0.9f);
  });
  scene->applyToObjects("mr_krabs", [](SceneObject *obj) {
    obj->setMaterialDiffuse(0.82f);
    obj->setMaterialSpecular(0.55f);
  });
  scene->applyToObjects("boid", [](SceneObject *obj) {
    obj->setMaterialDiffuse(0.7f);
    obj->setMaterialSpecular(0.65f);
  });

  scene->register_continuous_function(
      "boid", [](std::vector<SceneObject *> objs, float delta_time) {
        boid_iteration(objs, delta_time);
      });

  // Camera movement system
  scene->register_system([](Scene *scene, float delta_time) {
    Camera *camera = &scene->camera;
    // Simple gravity
    camera->addSpeed(glm::vec3(0.0f, -9.81f * delta_time, 0.0f));
    camera->updatePosition(delta_time);
    // Camera limits
    if (camera->getPosition().y < 0.5f) {
      camera->setPosition(
          glm::vec3(camera->getPosition().x, 0.5f, camera->getPosition().z));
      camera->setSpeed(
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
    if (camera->getPosition().y > 10.0f) {
      camera->setPosition(
          glm::vec3(camera->getPosition().x, 10.0f, camera->getPosition().z));
      camera->setSpeed(
          glm::vec3(camera->getVelocity().x, 0.0f, camera->getVelocity().z));
    }

    scene->applyToObjects("sky", [camera](SceneObject *obj) {
      obj->setPosition(camera->getPosition());
    });
  });

  // Jellyfish goes round and up and down
  // We store no state of the jellyfish beside transform, and therefore had to
  // derive all movement from it, which made things fun, it is also slowly going
  // away from the middle, but if the frame rate is very low it goes away
  // faster, as it is translated with the normal vector per tick with its
  // delta_time. So if delta_time -> 0 it is a perfect circle, but the higher it
  // gets, more wrong the circle is.
  scene->register_system([](Scene *scene, float delta_time) {
    scene->applyToObjects("jellyfish", [delta_time](SceneObject *obj) {
      // Goes round
      glm::vec3 normal2d =
          glm::vec3(obj->getPosition().z, 0.0f, -obj->getPosition().x);
      obj->translate(normal2d * 0.2f * delta_time);
      // Looks at the center
      obj->setRotation(
          glm::vec3(0.0f,
                    ((atan2(obj->getPosition().x, obj->getPosition().z)) *
                     180.0f / M_PI) +
                        90.0f,
                    0.0f));
      // Goes up and down
      obj->setPosition(
          glm::vec3(obj->getPosition().x,
                    sin(obj->getRotation().y * 2 * M_PI / 180.0f) * 2.0f,
                    obj->getPosition().z));
    });
  });

  // patrick looks at camera
  scene->register_system([](Scene *scene, float delta_time) {
    glm::vec3 camPos = scene->camera.getPosition();
    scene->applyToObjects("patrick", [camPos](SceneObject *obj) {
      obj->setRotation(glm::vec3(0.0f,
                                 atan2(camPos.x - obj->getPosition().x,
                                       camPos.z - obj->getPosition().z) *
                                     180.0f / M_PI,
                                 0.0f));
    });
  });

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
  inputSystem.registerKeyAction(
      GLFW_KEY_SPACE, [](Scene *scene, float delta_time) {
        scene->camera.setSpeed(glm::vec3(scene->camera.getVelocity().x, 10.0f,
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
      if (obj->getScale().x > 0.015f) {
        obj->setScale(glm::vec3(0.015f, 0.015f, 0.015f));
      }
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

  // Change ambient light intensity
  inputSystem.registerKeyAction(GLFW_KEY_1, [](Scene *scene, float delta_time) {
    scene->lighting.brighten_ambient(0.0052);
  });
  inputSystem.registerKeyAction(GLFW_KEY_2, [](Scene *scene, float delta_time) {
    scene->lighting.darken_ambient(0.005);
  });

  // Change specular strength
  inputSystem.registerKeyAction(GLFW_KEY_3, [](Scene *scene, float delta_time) {
    scene->lighting.increase_specular(0.005f);
  });
  inputSystem.registerKeyAction(GLFW_KEY_4, [](Scene *scene, float delta_time) {
    scene->lighting.decrease_specular(0.005f);
  });

  // Change diffuse strength
  inputSystem.registerKeyAction(GLFW_KEY_5, [](Scene *scene, float delta_time) {
    scene->lighting.increase_diffuse(0.005f);
  });
  inputSystem.registerKeyAction(GLFW_KEY_6, [](Scene *scene, float delta_time) {
    scene->lighting.decrease_diffuse(0.005f);
  });

  // Toggle lights on/off
  inputSystem.registerKeyAction(GLFW_KEY_R, [](Scene *scene, float delta_time) {
    scene->lighting.toggle_ambient();
  });
  inputSystem.registerKeyAction(GLFW_KEY_T, [](Scene *scene, float delta_time) {
    scene->lighting.toggle_specular();
  });
  inputSystem.registerKeyAction(GLFW_KEY_Y, [](Scene *scene, float delta_time) {
    scene->lighting.toggle_diffuse();
  });

  // Toggle object lights
  inputSystem.registerKeyAction(GLFW_KEY_U, [](Scene *scene, float delta_time) {
    scene->applyToObjects("jellyfish",
                          [](SceneObject *obj) { obj->toggleLight(); });
  });
  inputSystem.registerKeyAction(GLFW_KEY_I, [](Scene *scene, float delta_time) {
    scene->applyToObjects("anglerfish",
                          [](SceneObject *obj) { obj->toggleLight(); });
  });
  inputSystem.registerKeyAction(GLFW_KEY_O, [](Scene *scene, float delta_time) {
    scene->applyToObjects("small_lamp",
                          [](SceneObject *obj) { obj->toggleLight(); });
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