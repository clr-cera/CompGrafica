//
// Created by felipe on 29/05/2026.
//

#ifndef COMPGRAFICA_LIGHTING_H
#define COMPGRAFICA_LIGHTING_H

// Simple class to handle ambient lighing for now
// Even if it's just a single value, I choose to keep it isolated if it grows any bigger later

class Lighting {
public:

  Lighting(float initial_lighting, int location);

  // Ambient lighting changing
  void brighten_ambient(float increase);
  void darken_ambient(float decrease);
  float get_ambient();

private:
  // Ambient lighting internals
  float ambient_light;
  float ambient_uniform_location;
  void set_ambient_uniform();
};


#endif //COMPGRAFICA_LIGHTING_H