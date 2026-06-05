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
constexpr int BOID_COUNT = 10;
constexpr std::pair<float, float> BOID_SPAWN_XRANGE = {-4.0f, 4.0f};
constexpr std::pair<float, float> BOID_SPAWN_YRANGE = {2.2f, 3.4f};
constexpr std::pair<float, float> BOID_SPAWN_ZRANGE = {-4.0f, 4.0f};
constexpr std::pair<float, float> BOID_SPAWN_VEL_RANGE = {-0.01f, 0.01f};
constexpr float BOID_SCALE = 0.02f;

// Simulation constraints
constexpr float BOID_MIN_SPEED = 0.5f;
constexpr float BOID_MAX_SPEED = 1.0f;
constexpr float BOID_INNER_DISTANCE = 0.4f;
constexpr float BOID_OUTER_DISTANCE = 1.2f;

// Position bounds
constexpr std::pair<float, float> BOID_BOUNDS_X = {-8.0f, 8.0f};
constexpr std::pair<float, float> BOID_BOUNDS_Y = {2.2f, 4.0f};
constexpr std::pair<float, float> BOID_BOUNDS_Z = {-8.0f, 8.0f};

// Forces
constexpr float BOID_ATTRACTION_FORCE = 0.001f;
constexpr float BOID_REPULSION_FORCE = 0.5f;
constexpr float BOID_ALIGN_FORCE = 0.01f;

// Adds objects to the scene
void spawn_boids(Scene *scene, std::string obj_path, std::string texture_path);

// Iteration of the boid algo
// Should be called in a continuous uodate function
void boid_iteration(std::vector<SceneObject *> &boids, float delta_time);

// Makes the boid face its velocity direction
void align_boid_to_velocity(SceneObject *boid);

// Custom position update for boids with bounds
void update_boid_position(SceneObject *boid, float delta_time);

#endif // BOIDS_H