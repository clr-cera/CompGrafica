//
// Created by felipe on 17/03/26.
//

#define STB_IMAGE_IMPLEMENTATION
#include "obj_parser.hpp"
#include <sstream>

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
  std::vector<Vertex> vertices;
  std::vector<glm::vec2> texture_coords;
  std::vector<unsigned int> indices;

  int lineNumber = 0;
  while (std::getline(fileStream, line)) {
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
      std::vector<float> lineVertex;
      while (ss >> token) {
        try {
          lineVertex.push_back(std::stof(token));
        } catch (const std::invalid_argument &) {
          std::cerr << "ERROR::OBJ_PARSER::PARSE (Line " << lineNumber
                    << "): Invalid float value '" << token << "'" << std::endl;
          exit(1);
        } catch (const std::out_of_range &) {
          std::cerr << "ERROR::OBJ_PARSER::PARSE (Line " << lineNumber
                    << "): Float out of range '" << token << "'" << std::endl;
          exit(1);
        }
      }

      if (lineVertex.size() != 3 && lineVertex.size() != 6) {
        std::cerr << "ERROR::OBJ_PARSER::PARSE (Line " << lineNumber
                  << "): Vertex line must have 3 or 6 values, found "
                  << lineVertex.size() << std::endl;
        exit(1);
      }
      vertices.push_back(parseVertexLine(lineVertex));
      continue;
    }
    // Parse face line
    if (token == "f") {
      int count = 0;
      while (ss >> token) {
        try {
          unsigned int index = static_cast<unsigned int>(std::stoul(token));
          indices.push_back(index - 1);
          count++;
        } catch (const std::invalid_argument &) {
          std::cerr << "ERROR::OBJ_PARSER::PARSE (Line " << lineNumber
                    << "): Invalid face index '" << token << "'" << std::endl;
          exit(1);
        } catch (const std::out_of_range &) {
          std::cerr << "ERROR::OBJ_PARSER::PARSE (Line " << lineNumber
                    << "): Face index out of range '" << token << "'"
                    << std::endl;
          exit(1);
        }
      }
      if (count != 3) {
        std::cerr << "ERROR::OBJ_PARSER::PARSE (Line " << lineNumber
                  << "): Face line should have exactly 3 values, found "
                  << count << std::endl;
        exit(1);
      }
      continue;
    }
    // Parse texture line
    if (token == "vt") {
      std::vector<float> lineTexture;
      while (ss >> token) {
        try {
          lineTexture.push_back(std::stof(token));
        } catch (const std::invalid_argument &) {
          std::cerr << "ERROR::OBJ_PARSER::PARSE (Line " << lineNumber
                    << "): Invalid float value '" << token << "'" << std::endl;
          exit(1);
        } catch (const std::out_of_range &) {
          std::cerr << "ERROR::OBJ_PARSER::PARSE (Line " << lineNumber
                    << "): Float out of range '" << token << "'" << std::endl;
          exit(1);
        }
      }
      if (lineTexture.size() != 2) {
        std::cerr << "ERROR::OBJ_PARSER::PARSE (Line " << lineNumber
                  << "): Texture line must have 2 values, found "
                  << lineTexture.size() << std::endl;
        exit(1);
      }
      texture_coords.emplace_back(lineTexture[0], lineTexture[1]);
      continue;
    }
    std::cerr << "ERROR::OBJ_PARSER::PARSE: Unsupported or unknown line type. "
                 "Verify docs for supported ops"
              << std::endl;
    exit(1);
  }
  // Add texture to the vertices
  for (int i = 0; i < vertices.size(); i++) {
    vertices[i].texCoords = texture_coords[i];
  }
  // read texture
  auto texture = read_texture();
  return std::make_tuple(vertices, texture, indices);
}
