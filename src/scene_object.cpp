#include "scene_object.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "shader.hpp"

// Get or create the mesh on registry
SceneObject::SceneObject(std::string path, std::string texture_path) {
  mesh = MeshRegistry::getInstance().getMesh(path, texture_path);
}

// Transformation matrix caching, every time a transformation is updated,
// the matrix is marked as dirty and will be recalculated on the next call to
// transMatrix()
glm::mat4 SceneObject::transMatrix() {
  if (transMatrixNeedsUpdate) {
    glm::mat4 transMatrix = glm::mat4(1.0f);
    transMatrix = glm::translate(transMatrix, position);

    transMatrix = glm::rotate(transMatrix, glm::radians(rotation.x),
                              glm::vec3(1.0f, 0.0f, 0.0f));
    transMatrix = glm::rotate(transMatrix, glm::radians(rotation.y),
                              glm::vec3(0.0f, 1.0f, 0.0f));
    transMatrix = glm::rotate(transMatrix, glm::radians(rotation.z),
                              glm::vec3(0.0f, 0.0f, 1.0f));
    transMatrix = glm::scale(transMatrix, scale);

    cachedTransMatrix = transMatrix;
    transMatrixNeedsUpdate = false;
  }
  return cachedTransMatrix;
}

// Incremental transformations
void SceneObject::rotate(glm::vec3 rot) {
  rotation += rot;
  transMatrixNeedsUpdate = true;
}
void SceneObject::scaleUp(glm::vec3 sca) {
  scale += sca;
  transMatrixNeedsUpdate = true;
}
void SceneObject::translate(glm::vec3 tra) {
  position += tra;
  transMatrixNeedsUpdate = true;
}

void SceneObject::accelerate(glm::vec3 acc) {
  velocity += acc;
}

// Directly set transformations
void SceneObject::setRotation(glm::vec3 rot) {
  rotation = rot;
  transMatrixNeedsUpdate = true;
}
void SceneObject::setScale(glm::vec3 sca) {
  scale = sca;
  transMatrixNeedsUpdate = true;
}
void SceneObject::setPosition(glm::vec3 tra) {
  position = tra;
  transMatrixNeedsUpdate = true;
}

void SceneObject::setVelocity(glm::vec3 vel) {
  velocity = vel;
}

void SceneObject::updatePosition(float delta_time) {
  position += velocity * delta_time;
}

glm::vec3 SceneObject::getRotation() { return rotation; }
glm::vec3 SceneObject::getScale() { return scale; }
glm::vec3 SceneObject::getPosition() { return position; }

glm::vec3 SceneObject::getVelocity() {
  return velocity;
}

const void SceneObject::bind(Shader shader) {
  mesh->bind();
  shader.setMat4("transform", transMatrix());
}

const void SceneObject::unbind() { mesh->unbind(); }

const void SceneObject::draw(Shader shader) {
  bind(shader);
  glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
  unbind();
}