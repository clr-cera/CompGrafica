//
// Created by felipe on 17/03/26.
//

#define STB_IMAGE_IMPLEMENTATION
#include "obj_parser.hpp"
#include <sstream>
#include <map>

ObjFileParser::ObjFileParser(std::string filename, std::string path_to_texture) {
  this->fileStream = std::ifstream(filename);
  this->path_to_texture = path_to_texture;
  if (!fileStream.is_open()) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    exit(1);
  }
}

Vertex parseVertexLine(std::vector<float> lineArr) {
  if (lineArr.size() == 3) {
    return Vertex{glm::vec3(lineArr[0], lineArr[1], lineArr[2]),
                  glm::vec2(0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 0.0f)};
  }
  return Vertex{glm::vec3(lineArr[0], lineArr[1], lineArr[2]),
                glm::vec2(0.0f, 0.0f),
                glm::vec3(lineArr[3], lineArr[4], lineArr[5])};
}

Texture ObjFileParser::read_texture() {
  Texture texture;
  // stbi_set_flip_vertically_on_load(true);
  texture.data = stbi_load(path_to_texture.c_str(), &texture.width, &texture.height,
                           &texture.nrChannels, 0);
  if (texture.data == nullptr) {
    std::cerr << "ERROR::OBJ_PARSER::READ_TEXTURE: Failed to load texture"  << std::endl
              << stbi_failure_reason() << std::endl
              << std::endl;
    exit(1);
  }
  return texture;
}


std::tuple<std::vector<Vertex>, Texture, std::vector<unsigned int> >
ObjFileParser::parse() {
  std::string line;
  std::vector<glm::vec3> temp_positions;
  std::vector<glm::vec3> temp_colors;
  std::vector<glm::vec2> temp_tex_coords;

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::map<std::string, unsigned int> unique_vertices;

  int lineNumber = 0;
  while (std::getline(fileStream, line)) {
    lineNumber++;
    std::stringstream ss(line);
    std::string token;
    // Skip blank lines
    if (!(ss >> token)) {
      continue;
    }
    // Skip comments
    if (token[0] == '#') {
      continue;
    }
    // Parse vertex line
    if (token == "v") {
      float x, y, z;
      ss >> x >> y >> z;
      temp_positions.emplace_back(x, y, z);
      
      float r, g, b;
      if (ss >> r >> g >> b) {
          temp_colors.emplace_back(r, g, b);
      } else {
          temp_colors.emplace_back(0.0f, 0.0f, 0.0f);
      }
      continue;
    }
    // Parse texture line
    if (token == "vt") {
      float u, v;
      ss >> u >> v;
      temp_tex_coords.emplace_back(u, v);
      continue;
    }
    // Parse face line
    if (token == "f") {
      std::string vertexData;
      int faceVertices = 0;
      std::vector<unsigned int> faceIndices;
      while (ss >> vertexData) {
        if (unique_vertices.find(vertexData) == unique_vertices.end()) {
          std::stringstream vss(vertexData);
          std::string vIdxStr, vtIdxStr, vnIdxStr;
          
          std::getline(vss, vIdxStr, '/');
          std::getline(vss, vtIdxStr, '/');
          std::getline(vss, vnIdxStr, '/');

          if (vIdxStr.empty()) continue;

          unsigned int vIdx = std::stoul(vIdxStr) - 1;
          glm::vec2 texCoord(0.0f, 0.0f);
          if (!vtIdxStr.empty()) {
            unsigned int vtIdx = std::stoul(vtIdxStr) - 1;
            if (vtIdx < temp_tex_coords.size()) {
                texCoord = temp_tex_coords[vtIdx];
            }
          }

          Vertex v;
          v.position = temp_positions[vIdx];
          v.texCoords = texCoord;
          v.color = temp_colors[vIdx];

          unique_vertices[vertexData] = static_cast<unsigned int>(vertices.size());
          vertices.push_back(v);
        }
        faceIndices.push_back(unique_vertices[vertexData]);
        faceVertices++;
      }
      
      // Triangulate if it's a quad or polygon (fan triangulation)
      for (size_t i = 1; i < faceIndices.size() - 1; ++i) {
          indices.push_back(faceIndices[0]);
          indices.push_back(faceIndices[i]);
          indices.push_back(faceIndices[i + 1]);
      }

      if (faceVertices < 3) {
          std::cerr << "WARNING: Face at line " << lineNumber << " has fewer than 3 vertices." << std::endl;
      }
      continue;
    }
  }

  // read texture
  auto texture = read_texture();
  return std::make_tuple(vertices, texture, indices);
}
