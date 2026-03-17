#include "mesh.hpp"

#include <iostream>



Mesh::Mesh(std::string path_to_wavefront_obj) {
    // Parse wavefront object
    auto parser = ObjFileParser(path_to_wavefront_obj);
    auto [vertices, indices] = parser.parse();
    this->vertices = vertices;
    this->indices = indices;

    // Setup gpu vertex buffers
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    // Object VAO bound
    glBindVertexArray(VAO);

    // Vertex buffer bound and filled
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Element buffer bound and filled
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Setup buffer attributes

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Unbinds VAO
    glBindVertexArray(0);
}

void Mesh::bind() {
    glBindVertexArray(VAO);
}

void Mesh::unbind() {
    glBindVertexArray(0);
}
