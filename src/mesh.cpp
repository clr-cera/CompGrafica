#include "mesh.hpp"

Mesh::Mesh(std::string path_to_wavefront_obj, std::string path_to_texture) {
  // Parse wavefront object
  auto parser = ObjFileParser(path_to_wavefront_obj, path_to_texture);
  auto [vertices, texture, indices] = parser.parse();
  this->vertices = vertices;
  this->indices = indices;
  this->texture = texture;



  // Setup gpu vertex buffers
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);
  glGenTextures(1, &this->texture.id);

  // Texture creation
  glBindTexture(GL_TEXTURE_2D, this->texture.id);
  // set the texture wrapping/filtering options (on the currently bound texture object)
  // Needs some way to set this configs, but for now leave them hardcoded
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLenum format = GL_RGB;
  if (this->texture.nrChannels == 1)
    format = GL_RED;
  else if (this->texture.nrChannels == 3)
    format = GL_RGB;
  else if (this->texture.nrChannels == 4)
    format = GL_RGBA;

  glTexImage2D(GL_TEXTURE_2D, 0, format, this->texture.width, this->texture.height, 0, format, GL_UNSIGNED_BYTE, this->texture.data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(this->texture.data);

  // Object VAO bound
  glBindVertexArray(VAO);

  // Vertex buffer bound and filled
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  // Element buffer bound and filled
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  // Setup buffer attributes

  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  // Texture coordinates
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(float)));
  // color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)(5 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  // Unbinds VAO
  glBindVertexArray(0);
}

void Mesh::bind() const {
  glBindTexture(GL_TEXTURE_2D, texture.id);
  glBindVertexArray(VAO);
}

void Mesh::unbind() const {
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}

// Creates mesh if it doesnt exist
Mesh *MeshRegistry::getMesh(std::string path, std::string texture_path) {
  if (meshCache.find(path) == meshCache.end()) {
    meshCache[path] = Mesh(path, texture_path);
  }
  return &meshCache[path];
}
