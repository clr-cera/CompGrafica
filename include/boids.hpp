//
// Created by felipe on 24/04/2026.
//

#ifndef BOIDS_H
#define BOIDS_H
#include "scene.hpp"
#include <random>
#include <cmath>
// Adds objects to the scene
void spawn_boids(Scene *scene, int num_boids, std::string obj_path, std::string texture_path);

// Makes the boid face its velocity direction
void align_boid_to_velocity(SceneObject *boid);

#endif //BOIDS_H