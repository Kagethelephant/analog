#pragma once
// Program headers
#include "window/window.hpp"
#include "world/scene3D.hpp"
#include "shaders/shader.hpp"
#include "surface.hpp"
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Standard Libraries


/// @brief: GPU rendering engine to handle the OpenGL calls for 3D rendering for the provided
/// models / objects in 3D space. Draws to window FBO and window class handles rendering to GLFW window
class meshRenderer {

public:

   /// @brief: Create new GPU rendering engine
   /// @param cam: Camera to reference for view matrix and window size
   meshRenderer();

   /// @brief: Render the scene with loaded objects and lights to the window FBO
   void render(surface& surf, scene3D& scene);


private:
   
   /// @brief: OpenGL shader program comprised of vertex and fragment shaders pulled from the .glsl files
   GLuint m_shaderProgram3D;
};

