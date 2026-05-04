//
// Created by felipe on 24/04/2026.
//

#include "../include/boids.hpp"

#include <algorithm>
#include <numeric>
#include <random>

void spawn_boids(Scene *scene, std::string obj_path, std::string texture_path) {
  // Init rng and distributions for position/velocity
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
  // Generate
  for (int i = 0; i < BOID_COUNT; i++) {
    scene->addObject({"boid"}, obj_path, texture_path,
                     glm::vec3(dis_x(gen), dis_y(gen), dis_z(gen)),
                     glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(BOID_SCALE, BOID_SCALE, BOID_SCALE),
                     glm::vec3(dis_vel(gen), dis_vel(gen), dis_vel(gen)));
  }
}

void align_boid_to_velocity(SceneObject *boid) {
  glm::vec3 vel = boid->getVelocity();
  // Pythagoras for y and x+z, inversion due to the model orignal orientation
  float pitch = -1 * std::atan2(vel.y, std::sqrt(vel.x * vel.x + vel.z * vel.z));
  // Again, pythagoras, and inversion on X due to orignal model orientation
  float yaw = std::atan2(vel.z, -vel.x);
  yaw = glm::degrees(yaw);
  // fixed -90 because of orginal model orientation
  boid->setRotation(glm::vec3(-90.0f, -pitch, yaw));
}

void update_boid_position(SceneObject *boid, float delta_time) {
  glm::vec3 pos = boid->getPosition();
  glm::vec3 vel = boid->getVelocity();
  glm::vec3 new_pos = pos + (vel * delta_time);

  // Inverts velocity on bounds, avoid boids all over the scene
  if (new_pos.x < BOID_BOUNDS_X.first) {
    vel.x = std::abs(vel.x);
    new_pos.x = BOID_BOUNDS_X.first;
  } else if (new_pos.x > BOID_BOUNDS_X.second) {
    vel.x = -std::abs(vel.x);
    new_pos.x = BOID_BOUNDS_X.second;
  }

  if (new_pos.y < BOID_BOUNDS_Y.first) {
    vel.y = std::abs(vel.y);
    new_pos.y = BOID_BOUNDS_Y.first;
  } else if (new_pos.y > BOID_BOUNDS_Y.second) {
    vel.y = -std::abs(vel.y);
    new_pos.y = BOID_BOUNDS_Y.second;
  }

  if (new_pos.z < BOID_BOUNDS_Z.first) {
    vel.z = std::abs(vel.z);
    new_pos.z = BOID_BOUNDS_Z.first;
  } else if (new_pos.z > BOID_BOUNDS_Z.second) {
    vel.z = -std::abs(vel.z);
    new_pos.z = BOID_BOUNDS_Z.second;
  }

  boid->setVelocity(vel);
  boid->setPosition(new_pos);
}

// Util for eucliedean distance
float distance(SceneObject *a, SceneObject *b) {
  return glm::distance(a->getPosition(), b->getPosition());
}

// Boids are attracted to the center of mass of boids in it's outer range
void attraction_force(SceneObject *boid,
                      std::vector<SceneObject *> &neighbors) {
  // Center of mass through mean position (objects considered with same mass)
  glm::vec3 center_of_mass =
      std::accumulate(neighbors.begin(), neighbors.end(), glm::vec3(0.0f),
                      [](glm::vec3 acc, SceneObject *b) {
                        return acc + b->getPosition();
                      }) /
      static_cast<float>(neighbors.size());
  // If no neighbours, direct boid to center
  if (neighbors.size() == 0) {
    center_of_mass = glm::vec3(0.0f, 0.0f, 0.0f);
  }
  // Update velocity
  glm::vec3 force_vector =
      (center_of_mass - boid->getPosition()) * BOID_ATTRACTION_FORCE;
  boid->accelerate(force_vector);
}

void repulsion_force(SceneObject *boid, std::vector<SceneObject *> &neighbors) {
  // Center of mass through mean position (objects considered with same mass)
  glm::vec3 center_of_mass =
      std::accumulate(neighbors.begin(), neighbors.end(), glm::vec3(0.0f),
                      [](glm::vec3 acc, SceneObject *b) {
                        return acc + b->getPosition();
                      }) /
      static_cast<float>(neighbors.size());
  // If no neighbours, no force
  if (neighbors.size() == 0) {
    return;
  }
  // Update velocity
  glm::vec3 force_vector =
      (boid->getPosition() - center_of_mass) * BOID_REPULSION_FORCE;
  boid->accelerate(force_vector);
}

void alignment_force(SceneObject *boid, std::vector<SceneObject *> &neighbors) {
  // Center of mass through mean position (objects considered with same mass)
  glm::vec3 average_velocity =
      std::accumulate(neighbors.begin(), neighbors.end(), glm::vec3(0.0f),
                      [](glm::vec3 acc, SceneObject *b) {
                        return acc + b->getVelocity();
                      }) /
      static_cast<float>(neighbors.size());
  // If no neighbours, no force
  if (neighbors.size() == 0) {
    return;
  }

  // Update velocity direction but not magnitude, to keep movement smooth
  float current_speed = glm::length(boid->getVelocity());
  if (current_speed > 0) {
    glm::vec3 steer = average_velocity * BOID_ALIGN_FORCE;
    glm::vec3 new_velocity = boid->getVelocity() + steer;
    if (glm::length(new_velocity) > 0) {
      new_velocity = glm::normalize(new_velocity) * current_speed;
      boid->setVelocity(new_velocity);
    }
  }
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
    alignment_force(boid, neighbors);
  }

  // Update boid direction and position
  for (auto boid : boids) {
    // Clamp velocity to min and max speed
    glm::vec3 vel = boid->getVelocity();
    float speed = glm::length(vel);
    if (speed > BOID_MAX_SPEED) {
      boid->setVelocity(glm::normalize(vel) * BOID_MAX_SPEED);
    } else if (speed < BOID_MIN_SPEED && speed > 0) {
      boid->setVelocity(glm::normalize(vel) * BOID_MIN_SPEED);
    } else if (speed == 0) {
      // If speed is 0, give it a small random velocity or just min speed in some direction
      boid->setVelocity(glm::vec3(0.0f, 0.01f, 0.0f)); // Simple fallback
    }

    update_boid_position(boid, delta_time);
    align_boid_to_velocity(boid);
  }
}
