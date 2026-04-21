#ifndef MESH_HPP
#define MESH_HPP
#include "glad/glad.h"
#include "obj_parser.hpp"
#include <glm/glm.hpp>
#include <string>
#include <vector>

// Meshes contains all geometric data, and is already setup to the gpu on
// construction.
class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  Texture texture;

  unsigned int VAO, VBO, EBO;

  Mesh() : VAO(0), VBO(0), EBO(0) {}
  Mesh(std::string path_to_wavefront_obj, std::string path_to_texture);
  void bind() const;
  void unbind() const;
};

// Singleton to cache meshes if the path is repeated
// We assumed that paths are not changed after the first creation
class MeshRegistry {
public:
  // Get singleton
  static MeshRegistry &getInstance() {
    static MeshRegistry instance;
    return instance;
  }

  Mesh *getMesh(std::string path, std::string texture_path);

  // Remove creating more objects or assignment
  MeshRegistry(const MeshRegistry &) = delete;
  void operator=(const MeshRegistry &) = delete;

private:
  MeshRegistry() {}
  std::unordered_map<std::string, Mesh> meshCache;
};
#endif // MESH_HPP