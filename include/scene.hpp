#ifndef SCENE_HPP
#define SCENE_HPP

#include "scene_object.hpp"
#include "shader.hpp"
#include <string>

class Scene {
public:
  Shader shader;
  std::vector<SceneObject> objects;
  Scene(std::string vertexShaderPath, std::string fragmentShaderPath,
        std::vector<std::string> objectPaths);

  void Render();
};
#endif // SCENE_HPP