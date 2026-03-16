//
// Created by felipe on 16/03/26.
//
#include "shader.hpp"

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath) {

  // Shader input from files

  std::string vertexShaderCode;
  std::string fragmentShaderCode;
  std::ifstream vShaderFile(vertexShaderPath);
  std::ifstream fShaderFile(fragmentShaderPath);

  // Verify file exists
  if (vShaderFile.fail()) {
    std::perror("Error: ");
  }
  if (fShaderFile.fail()) {
    std::perror("Error: ");
  }

  // File reading
  std::stringstream vShaderStream, fShaderStream;
  vShaderStream << vShaderFile.rdbuf();
  fShaderStream << fShaderFile.rdbuf();
  vShaderFile.close();
  fShaderFile.close();
  vertexShaderCode = vShaderStream.str();
  fragmentShaderCode = fShaderStream.str();

  const char *vShaderCode = vertexShaderCode.c_str();
  const char *fShaderCode = fragmentShaderCode.c_str();

  // Shader Compilation

  unsigned int vertexID, fragmentID;
  int success;
  char infoLog[512];

  // vertex Shader
  vertexID = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexID, 1, &vShaderCode, NULL);
  glCompileShader(vertexID);
  // print compile errors if any
  glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  };

  // fragment shader
  fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentID, 1, &fShaderCode, NULL);
  glCompileShader(fragmentID);
  // print compile errors if any
  glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  };

  // shader Program
  programID = glCreateProgram();
  glAttachShader(programID, vertexID);
  glAttachShader(programID, fragmentID);
  glLinkProgram(programID);
  // print linking errors if any
  glGetProgramiv(programID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(programID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  // delete the shaders as they're linked into our program now and no longer
  // necessary
  glDeleteShader(vertexID);
  glDeleteShader(fragmentID);
}

void Shader::use() const { glUseProgram(programID); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}
void Shader::setMat4(const std::string name, glm::mat4 mat) const {
  glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(mat));
}
