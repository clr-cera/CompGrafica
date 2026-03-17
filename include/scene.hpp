#ifndef SCENE_HPP
#define SCENE_HPP

#include "scene_object.hpp"
#include "shader.hpp"
#include <map>
#include <string>

class Scene {
public:
  Shader shader;
  std::map<std::string, SceneObject> objects;
  Scene(std::string vertexShaderPath, std::string fragmentShaderPath,
        std::vector<std::string> objectPaths,
        std::vector<std::string> objectNames);

  void Render();
};
#endif // SCENE_HPP