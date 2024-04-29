# Project settings
cmake_minimum_required(VERSION 3.5)
project(practica0)


# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)


# Required libraries installed in the system

## OpenGL
set(OpenGL_GL_PREFERENCE GLVND) # Set OpenGL version
find_package(OpenGL REQUIRED)

## GLFW3
find_package(glfw3 3.3 REQUIRED)

## GLEW
find_package(GLEW REQUIRED)

## GLM
find_package(glm REQUIRED)
## Assimp
find_package(assimp REQUIRED)
# Source files
add_executable(
  ${PROJECT_NAME} src/practica7.cpp
  
  src/Camera.cpp
  src/DirectionalLight.cpp
  src/Light.cpp
  src/Material.cpp
  src/Mesh_tn.cpp
  src/Model.cpp
  src/PointLight.cpp
  src/Shader_light.cpp
  src/Skybox.cpp
  src/Sphere.cpp
  src/SpotLight.cpp
  src/Window.cpp
  src/Texture.cpp


    shaders/shader_light.vert
   shaders/shader_light.frag
  shaders/skybox.vert
  shaders/skybox.frag
  
)
include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
# Linked libraries settings
target_link_libraries(${PROJECT_NAME}
  OpenGL::GL
  GLEW
  glfw
  glm::glm
  assimp

  Xrandr
  Xi
  Xinerama
  Xcursor
)
