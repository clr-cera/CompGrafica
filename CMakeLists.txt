cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
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

add_executable(Graphics src/main.cpp  src/obj_parser.cpp )
#add_executable(Graphics src/main.cpp  src/mesh.cpp  src/obj_parser.cpp  src/scene.cpp  src/scene_object.cpp  src/shader.cpp)
# Setup asset directories
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
		DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/objects"
		DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

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
