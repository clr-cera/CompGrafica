#ifndef MESH_HPP
#define MESH_HPP
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Mesh {
public:
  struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoords;
  };
  struct Texture {
    unsigned int id;
  };

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  unsigned int VAO, VBO, EBO;

  Mesh(std::string path_to_wavefront_obj);

private:
  void setupMesh();
};
#endif // MESH_HPP