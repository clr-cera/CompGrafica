//
// Created by felipe on 17/03/26.
//

#include "obj_parser.h"


ObjFileParser::ObjFileParser(std::string filename) {
    fileStream = std::ifstream(filename);
    if (!fileStream.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        exit(1);
    }
}


Vertex parseVertexLine(std::vector<float> lineArr) {
    if (lineArr.size() == 3) {
        return Vertex {
          glm::vec3(lineArr[0], lineArr[1], lineArr[2]),
          // glm::vec2(0.0f, 0.0f),
          glm::vec3(0.0f, 0.0f, 0.0f)
        };
    }
    return Vertex {
        glm::vec3(lineArr[0], lineArr[1], lineArr[2]),
        // glm::vec2(0.0f, 0.0f),
        glm::vec3(lineArr[3], lineArr[4], lineArr[5])
      };
}


std::pair<std::vector<Vertex>, std::vector<unsigned int>> ObjFileParser::parse() {
    std::string line;
    std::vector<Vertex> vertices;
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
                } catch (const std::invalid_argument&) {
                    std::cerr << "ERROR::OBJ_PARSER::PARSE (Line " << lineNumber
                              << "): Invalid float value '" << token << "'" << std::endl;
                    exit(1);
                } catch (const std::out_of_range&) {
                    std::cerr << "ERROR::OBJ_PARSER::PARSE (Line " << lineNumber
                              << "): Float out of range '" << token << "'" << std::endl;
                    exit(1);
                }
            }

            if (lineVertex.size() != 3 && lineVertex.size() != 6) {
                std::cerr << "ERROR::OBJ_PARSER::PARSE (Line " << lineNumber
                          << "): Vertex line must have 3 or 6 values, found " << lineVertex.size() << std::endl;
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
                } catch (const std::invalid_argument&) {
                    std::cerr << "ERROR::OBJ_PARSER::PARSE (Line " << lineNumber
                              << "): Invalid face index '" << token << "'" << std::endl;
                    exit(1);
                } catch (const std::out_of_range&) {
                    std::cerr << "ERROR::OBJ_PARSER::PARSE (Line " << lineNumber
                              << "): Face index out of range '" << token << "'" << std::endl;
                    exit(1);
                }
            }
            if (count != 3) {
                std::cerr << "ERROR::OBJ_PARSER::PARSE (Line " << lineNumber
                          << "): Face line should have exactly 3 values, found " << count << std::endl;
                exit(1);
            }
            continue;
        }
        std::cerr << "ERROR::OBJ_PARSER::PARSE: Unsupported or unknown line type. Verify docs for supported ops" << std::endl;
        exit(1);
    }
    return std::make_pair(vertices, indices);
}


