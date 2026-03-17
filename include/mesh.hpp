#ifndef MESH_HPP
#define MESH_HPP
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "obj_parser.h"
#include "glad/glad.h"

class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  unsigned int VAO, VBO, EBO;

  Mesh(std::string path_to_wavefront_obj);
    void bind();
    void unbind();
};
#endif // MESH_HPP