#ifndef MESH_HPP
#define MESH_HPP
#include "glad/glad.h"
#include "obj_parser.hpp"
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  unsigned int VAO, VBO, EBO;

  Mesh() : VAO(0), VBO(0), EBO(0) {}
  Mesh(std::string path_to_wavefront_obj);
  void bind() const;
  void unbind() const;
};
#endif // MESH_HPP