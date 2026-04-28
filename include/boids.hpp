//
// Created by felipe on 24/04/2026.
//

#ifndef BOIDS_H
#define BOIDS_H
#include "scene.hpp"
// ========================================================================
// Constants - Tune here the params fot the boids simultation
// ========================================================================

// Boid spawning
constexpr int BOID_COUNT = 50;
constexpr std::pair<float, float> BOID_SPAWN_XRANGE = {-0.8f, 0.8f};
constexpr std::pair<float, float> BOID_SPAWN_YRANGE = {-0.1f, 0.1f};
constexpr std::pair<float, float> BOID_SPAWN_ZRANGE = {-0.8f, 0.8f};
constexpr std::pair<float, float> BOID_SPAWN_VEL_RANGE = {-0.01f, 0.01f};

// Simulation constraints
constexpr float BOID_MAX_SPEED = 0.05f;
constexpr float BOID_INNER_DISTANCE = 0.2f;
constexpr float BOID_OUTER_DISTANCE = 0.6f;

// Forces
constexpr float BOID_ATTRACTION_FORCE = 0.005f;
constexpr float BOID_REPULSION_FORCE = 0.1f;
constexpr float BOID_ALIGN_FORCE = 0.1f;

// Adds objects to the scene
void spawn_boids(Scene *scene, std::string obj_path, std::string texture_path);

// Iteration of the boid algo
void boid_iteration(std::vector<SceneObject *> &boids, float delta_time);

// Makes the boid face its velocity direction
void align_boid_to_velocity(SceneObject *boid);

#endif // BOIDS_H