//
// Created by felipe on 16/03/26.
//

#ifndef SHADER_H
#define SHADER_H
#include "../dependencies/glad/include/glad/glad.h"
#include <cstring>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  // Program ID
  unsigned int programID;

  // Constructor handles shader reading, compiling and linking
  Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
  ~Shader();

  // Uses the program
  void use() const;

  // Utility uniform setting functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMat4(const std::string name, glm::mat4 mat) const;
};

#endif // SHADER_H
