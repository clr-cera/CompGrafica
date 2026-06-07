//
// Created by felipe on 29/05/2026.
//

#ifndef COMPGRAFICA_LIGHTING_H
#define COMPGRAFICA_LIGHTING_H

#include "glm/glm.hpp"

// Simple class to handle ambient lighing for now
// Even if it's just a single value, I choose to keep it isolated if it grows
// any bigger later

class Lighting {
public:
  Lighting(float initial_lighting, int ambient_location,
           int ambient_color_location, int specular_location,
           int diffuse_location);

  // Ambient lighting changing
  void brighten_ambient(float increase);
  void darken_ambient(float decrease);
  void toggle_ambient();
  float get_ambient();

  // Specular lighting changing
  void increase_specular(float increase);
  void decrease_specular(float decrease);
  void toggle_specular();
  float get_specular();

  // Diffuse lighting changing
  void increase_diffuse(float increase);
  void decrease_diffuse(float decrease);
  void toggle_diffuse();
  float get_diffuse();

  void set_ambient_uniform();

private:
  // These values are used to render each type of light separately, and to
  // toggle/change them with commands
  float ambient_light;
  float ambient_uniform_location;
  glm::vec3 ambient_color;
  float ambient_color_uniform_location;
  float ambient_light_saved;
  float ambient_last_toggle_time = 0.0f;

  float specular_strength;
  float specular_uniform_location;
  float specular_strength_saved;
  float specular_last_toggle_time = 0.0f;

  float diffuse_strength;
  float diffuse_uniform_location;
  float diffuse_strength_saved;
  float diffuse_last_toggle_time = 0.0f;
};

#endif // COMPGRAFICA_LIGHTING_H