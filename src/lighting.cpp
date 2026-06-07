//
// Created by felipe on 29/05/2026.
//

#include "lighting.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <algorithm>

void Lighting::set_ambient_uniform() {
  glUniform1f(ambient_uniform_location, ambient_light);
  glUniform3f(ambient_color_uniform_location, ambient_color.x, ambient_color.y,
              ambient_color.z);
  glUniform1f(specular_uniform_location, specular_strength);
  glUniform1f(diffuse_uniform_location, diffuse_strength);
}

Lighting::Lighting(float initial_ambient, int ambient_location,
                   int color_uniform_location, int specular_location,
                   int diffuse_location)
    : ambient_light(initial_ambient),
      ambient_uniform_location(ambient_location),
      ambient_color(glm::vec3(1.0f, 1.0f, 1.0f)),
      ambient_color_uniform_location(color_uniform_location),
      ambient_light_saved(initial_ambient),
      specular_strength(0.5f),
      specular_uniform_location(specular_location),
      specular_strength_saved(0.5f),
      diffuse_strength(1.0f),
      diffuse_uniform_location(diffuse_location),
      diffuse_strength_saved(1.0f) {
  set_ambient_uniform();
}

void Lighting::brighten_ambient(float increase) {
  ambient_light = std::min(1.0f, ambient_light + increase);
  set_ambient_uniform();
}

void Lighting::darken_ambient(float decrease) {
  ambient_light = std::max(0.0f, ambient_light - decrease);
  set_ambient_uniform();
}

float Lighting::get_ambient() { return ambient_light; }

void Lighting::increase_specular(float increase) {
  specular_strength = std::min(1.0f, specular_strength + increase);
  set_ambient_uniform();
}

void Lighting::decrease_specular(float decrease) {
  specular_strength = std::max(0.0f, specular_strength - decrease);
  set_ambient_uniform();
}

float Lighting::get_specular() { return specular_strength; }

void Lighting::increase_diffuse(float increase) {
  diffuse_strength = std::min(1.0f, diffuse_strength + increase);
  set_ambient_uniform();
}

void Lighting::decrease_diffuse(float decrease) {
  diffuse_strength = std::max(0.0f, diffuse_strength - decrease);
  set_ambient_uniform();
}

float Lighting::get_diffuse() { return diffuse_strength; }

void Lighting::toggle_ambient() {
  if (ambient_last_toggle_time + 0.2f > glfwGetTime()) {
    return;
  }
  ambient_last_toggle_time = glfwGetTime();
  if (ambient_light > 0.0f) {
    ambient_light_saved = ambient_light;
    ambient_light = 0.0f;
  } else {
    ambient_light = ambient_light_saved;
  }
  set_ambient_uniform();
}

void Lighting::toggle_specular() {
  if (specular_last_toggle_time + 0.2f > glfwGetTime()) {
    return;
  }
  specular_last_toggle_time = glfwGetTime();
  if (specular_strength > 0.0f) {
    specular_strength_saved = specular_strength;
    specular_strength = 0.0f;
  } else {
    specular_strength = specular_strength_saved;
  }
  set_ambient_uniform();
}

void Lighting::toggle_diffuse() {
  if (diffuse_last_toggle_time + 0.2f > glfwGetTime()) {
    return;
  }
  diffuse_last_toggle_time = glfwGetTime();
  if (diffuse_strength > 0.0f) {
    diffuse_strength_saved = diffuse_strength;
    diffuse_strength = 0.0f;
  } else {
    diffuse_strength = diffuse_strength_saved;
  }
  set_ambient_uniform();
}
