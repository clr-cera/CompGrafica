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

// Singleton to cache meshes if the path is repeated
class MeshRegistry {
public:
  // Get singleton
  static MeshRegistry &getInstance() {
    static MeshRegistry instance;
    return instance;
  }

  Mesh *getMesh(std::string path);

  // Remove creating more objects or assignment
  MeshRegistry(const MeshRegistry &) = delete;
  void operator=(const MeshRegistry &) = delete;

private:
  MeshRegistry() {}
  std::unordered_map<std::string, Mesh> meshCache;
};
#endif // MESH_HPP