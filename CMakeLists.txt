cmake_minimum_required(VERSION 3.10)
project(CompGrafica VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(dependencies/glfw)

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
)

# Link libraries
target_link_libraries(Graphics
    glfw
    glad
    ${OPENGL_LIBRARIES}
)
