//
// Created by felipe on 24/04/2026.
//

#include "../include/boids.hpp"


void spawn_boids(Scene *scene, int num_boids, std::string obj_path, std::string texture_path) {
  // Init rng
  std:: random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis_x(-0.5, 0.5);
  std::uniform_real_distribution<> dis_z(-0.5, 0.5);
  std::uniform_real_distribution<> dis_y(-0.2, 0.2);

  for (int i = 0; i < num_boids; i++) {
    scene->addObject(
      {"boid", "has_velocity"}, obj_path, texture_path,
      glm::vec3(dis_x(gen), dis_y(gen), dis_z(gen)),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.5f, 0.5f, 0.5f),
      glm::vec3(0.01f, 0.00f, 0.02f)
    );
  }
}


void align_boid_to_velocity(SceneObject *boid) {
  glm::vec3 vel = boid->getVelocity();
  float pitch = std::atan2(vel.y, std::sqrt(vel.x * vel.x + vel.z * vel.z));
  pitch = -1 * glm::degrees(pitch);
  // std::cout << pitch << std::endl;
  boid->setRotation(glm::vec3(pitch, 0.0f, 0.0f));
  float yaw = std::atan2(vel.x, vel.z);
  yaw = glm::degrees(yaw);
  boid->rotate(glm::vec3(0.0f, yaw, 0.0f));
}


