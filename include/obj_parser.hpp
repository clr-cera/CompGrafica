//
// Created by felipe on 17/03/26.
//

#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


struct Texture {
  unsigned int id;
  unsigned char* data;
  int width, height, nrChannels;
};

struct Vertex {
  glm::vec3 position;
  glm::vec2 texCoords;
  glm::vec3 color;
  // The "Quickest" way: Overload the insertion operator
  friend std::ostream &operator<<(std::ostream &os, const Vertex &v) {
    os << "[Pos: " << v.position.x << ", " << v.position.y << ", "
       << v.position.z
       << "] "
       << "[UV: "  << v.texCoords.x << ", " << v.texCoords.y << "] "
       << "[Col: " << v.color.r << ", " << v.color.g << ", " << v.color.b
       << "]";
    return os;
  }
};

/*
 Parser Version 1, supports .obj files with comments, vertices and faces
 v lines can have 3 or 6 floats (x, y, z, r, g, b)
 f lines should have 3 integers (no /)
 including other types of lines will cause an error
 */
class ObjFileParser {
public:
  ObjFileParser(std::string filename, std::string path_to_texture);
  std::ifstream fileStream;
  std::string path_to_texture;
  std::tuple<std::vector<Vertex>, Texture, std::vector<unsigned int>> parse();

private:
  Texture read_texture();
};
#endif // OBJ_PARSER_H
