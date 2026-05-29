//
// Created by felipe on 29/05/2026.
//

#include "lighting.hpp"
#include "glad/glad.h"

#include <algorithm>

void Lighting::set_ambient_uniform() {
  glUniform1f(ambient_uniform_location, ambient_light);
}

Lighting::Lighting(float initial_ambient, int location)
: ambient_light(initial_ambient), ambient_uniform_location(location) {
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

float Lighting::get_ambient() {
  return ambient_light;
}


