#include "render.hpp"
// Program headers
#include "RAIIWrapper.hpp"
#include "glm/fwd.hpp"
#include "window/window.hpp"
#include "window/text.hpp"
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Std Libraries
#include <vector>



gpuRenderEngine::gpuRenderEngine( const window& win) : m_window{win}{ 

   shaderProgramUI = createShaderProgram("../src/shaders/ui_vertex.glsl", "../src/shaders/ui_fragment.glsl");


   // DEFINE THE VERTEX DATA QUAD
   // -----------------------------------------------------------------------------------
   m_quad = {
      -1.0f,  1.0f,  0.0f,  1.0f, // x, y, u, v
      -1.0f, -1.0f,  0.0f,  0.0f,
       1.0f, -1.0f,  1.0f,  0.0f,

      -1.0f,  1.0f,  0.0f,  1.0f,
       1.0f, -1.0f,  1.0f,  0.0f,
       1.0f,  1.0f,  1.0f,  1.0f
   };

   // Create the VAO storing the vertice data for our full screen quad
   glGenVertexArrays(1, &m_quadVao);  
   glGenBuffers(1, &m_quadVbo);

   // Setup the VBO using the VAO
   GLScopedVAO tempVAO(m_quadVao);
   GLScopedVBO tempVBO(m_quadVbo);

   glBufferData(GL_ARRAY_BUFFER, m_quad.size() * sizeof(GLfloat), m_quad.data(), GL_STATIC_DRAW);
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
   // This tells GL to use the vertex attributes defined above (it does not do this by default)
   glEnableVertexAttribArray(0);  
   glEnableVertexAttribArray(1);  
};



void gpuRenderEngine::draw(surface& surf){

   // Render the screen
   // Bind the main window framebuffer to draw to the screen
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   // Set the viewport, shader program and VAO with RAII wrappers that will reset to previous
   // OpenGL states at the end of the scope where they were bound
   GLScopedViewport winViewPort(0, 0, m_window.getWindowSize().x, m_window.getWindowSize().y);
   GLScopedProgram winProgram(shaderProgramUI);
   GLScopedVAO tempVAO(m_quadVao);
   // Select texture unit 0 and bind colorTex to GL_TEXTURE_2D on that unit
   glActiveTexture(GL_TEXTURE0);
   GLScopedTexture2D tempTexture(surf.getSurface());

   glUniform1i(glGetUniformLocation(shaderProgramUI, "screenTexture"), 0);
   glDrawArrays(GL_TRIANGLES, 0, 6);
}

