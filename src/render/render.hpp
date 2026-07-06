#pragma once
// Program headers
#include "window/window.hpp"
#include "shaders/shader.hpp"
#include "surface.hpp"
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Standard Libraries
#include <vector>


/// @brief: GPU rendering engine to handle the OpenGL calls for 3D rendering for the provided
/// models / objects in 3D space. Draws to window FBO and window class handles rendering to GLFW window
class gpuRenderEngine {

public:

   /// @brief: Create new GPU rendering engine
   /// @param cam: Camera to reference for view matrix and window size
   gpuRenderEngine(const window& win);

   void draw(surface& surf);

private:
   
   /// @brief: Shader program to render 2D quads with textures
   GLuint shaderProgramUI;

   const window& m_window;

   GLuint m_quadVao;
   GLuint m_quadVbo;

   std::vector<float> m_quad;
};

