cmake_minimum_required(VERSION 3.10)
project(CompGrafica VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)

add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glm)

find_package(OpenGL REQUIRED)

# Add GLAD
add_library(glad STATIC dependencies/glad/src/glad.c)
target_include_directories(glad PUBLIC dependencies/glad/include)

add_executable(Graphics src/main.cpp)

# Include directories
target_include_directories(Graphics PRIVATE
    ${OPENGL_INCLUDE_DIR}
    dependencies/glfw/include
    dependencies/glad/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(Graphics
    glfw
    glad
  glm::glm
    ${OPENGL_LIBRARIES}
)
