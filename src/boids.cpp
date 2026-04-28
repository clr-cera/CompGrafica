//
// Created by felipe on 24/04/2026.
//

#include "../include/boids.hpp"

#include <algorithm>
#include <numeric>
#include <random>

void spawn_boids(Scene *scene, std::string obj_path, std::string texture_path,
                 glm::vec3 initial_position) {
  // Init rng
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis_x(BOID_SPAWN_XRANGE.first,
                                         BOID_SPAWN_XRANGE.second);
  std::uniform_real_distribution<> dis_y(BOID_SPAWN_YRANGE.first,
                                         BOID_SPAWN_YRANGE.second);
  std::uniform_real_distribution<> dis_z(BOID_SPAWN_ZRANGE.first,
                                         BOID_SPAWN_ZRANGE.second);
  std::uniform_real_distribution<> dis_vel(BOID_SPAWN_VEL_RANGE.first,
                                           BOID_SPAWN_VEL_RANGE.second);

  for (int i = 0; i < BOID_COUNT; i++) {
    scene->addObject({"boid"}, obj_path, texture_path,
                     initial_position +
                         glm::vec3(dis_x(gen), dis_y(gen), dis_z(gen)),
                     glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f),
                     glm::vec3(dis_vel(gen), dis_vel(gen), dis_vel(gen)));
  }
}

void align_boid_to_velocity(SceneObject *boid) {
  glm::vec3 vel = boid->getVelocity();
  float pitch = std::atan2(vel.y, std::sqrt(vel.x * vel.x + vel.z * vel.z));
  pitch = -1 * glm::degrees(pitch);
  // std::cout << pitch << std::endl;
  boid->setRotation(glm::vec3(0.0f, 0.0f, pitch));
  float yaw = std::atan2(vel.x, vel.z);
  yaw = glm::degrees(yaw);
  boid->setRotation(glm::vec3(0.0f, yaw, 0.0f));
}

float distance(SceneObject *a, SceneObject *b) {
  return glm::distance(a->getPosition(), b->getPosition());
}

void attraction_force(SceneObject *boid,
                      std::vector<SceneObject *> &neighbors) {
  glm::vec3 center_of_mass =
      std::accumulate(neighbors.begin(), neighbors.end(), glm::vec3(0.0f),
                      [](glm::vec3 acc, SceneObject *b) {
                        return acc + b->getPosition();
                      }) /
      static_cast<float>(neighbors.size());
  if (neighbors.size() == 0) {
    center_of_mass = glm::vec3(0.0f, 0.0f, 0.0f);
  }
  glm::vec3 force_vector =
      (center_of_mass - boid->getPosition()) * BOID_ATTRACTION_FORCE;
  boid->accelerate(force_vector);
}

void repulsion_force(SceneObject *boid, std::vector<SceneObject *> &neighbors) {
  glm::vec3 center_of_mass =
      std::accumulate(neighbors.begin(), neighbors.end(), glm::vec3(0.0f),
                      [](glm::vec3 acc, SceneObject *b) {
                        return acc + b->getPosition();
                      }) /
      static_cast<float>(neighbors.size());
  if (neighbors.size() == 0) {
    return;
  }
  glm::vec3 force_vector =
      (boid->getPosition() - center_of_mass) * BOID_REPULSION_FORCE;
  boid->accelerate(force_vector);
}

void alignment_force(SceneObject *boid, std::vector<SceneObject *> &neighbors) {
  glm::vec3 center_of_mass =
      std::accumulate(neighbors.begin(), neighbors.end(), glm::vec3(0.0f),
                      [](glm::vec3 acc, SceneObject *b) {
                        return acc + b->getVelocity();
                      }) /
      static_cast<float>(neighbors.size());
  if (neighbors.size() == 0) {
    return;
  }
  glm::vec3 force_vector =
      (center_of_mass - boid->getVelocity()) * BOID_ALIGN_FORCE;
  boid->accelerate(force_vector);
}

// It can probably be optimized with more elegant algorithms, but the simulation
// will be small So who cares
void boid_iteration(std::vector<SceneObject *> &boids, float delta_time) {
  // Calculate forces acting on each boid
  for (auto boid : boids) {
    // Attraction to neighbors
    std::vector<SceneObject *> neighbors;
    std::copy_if(boids.begin(), boids.end(), std::back_inserter(neighbors),
                 [&](SceneObject *other_boid) {
                   float d = distance(boid, other_boid);
                   return (other_boid != boid) & (d < BOID_OUTER_DISTANCE) &
                          (d > BOID_INNER_DISTANCE);
                 });
    attraction_force(boid, neighbors);

    // Repulsion to neighbors too close
    std::vector<SceneObject *> close_neighbors;
    std::copy_if(boids.begin(), boids.end(),
                 std::back_inserter(close_neighbors),
                 [&](SceneObject *other_boid) {
                   float d = distance(boid, other_boid);
                   return (other_boid != boid) & (d < BOID_INNER_DISTANCE);
                 });
    repulsion_force(boid, close_neighbors);

    // Velocity alignment
  }

  // Update boid direction and position
  for (auto boid : boids) {
    boid->updatePosition(delta_time);
    auto pos = boid->getPosition();
    align_boid_to_velocity(boid);
  }
}
