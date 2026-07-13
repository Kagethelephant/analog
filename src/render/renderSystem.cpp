#include "renderSystem.hpp"
// Program headers
#include "RAIIWrapper.hpp"
#include "glm/fwd.hpp"
#include "window/window.hpp"
#include "render/textRenderer.hpp"
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Std Libraries
#include <vector>



renderSystem::renderSystem( const window& win) : m_window{win}{ 

   m_shaderProgramUI = createShaderProgram("../src/shaders/ui_vertex.glsl", "../src/shaders/ui_fragment.glsl");

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

void renderSystem::blit(surface& surf, const frameBuffer& target){
   glBindFramebuffer(GL_FRAMEBUFFER, target.framebuffer());
   GLScopedViewport winViewPort(0, 0, m_window.getWindowSize().x, m_window.getWindowSize().y);
   render(surf);
}

void renderSystem::blit(surface& surf, const frameBuffer& target, float x, float y){
   glBindFramebuffer(GL_FRAMEBUFFER, target.framebuffer());
   GLScopedViewport winViewPort(x, y, surf.size().x, surf.size().y);
   render(surf);
}

void renderSystem::blit(surface& surf, const frameBuffer& target, float x, float y, float w, float h){
   glBindFramebuffer(GL_FRAMEBUFFER, target.framebuffer());
   GLScopedViewport winViewPort(x,y,w,h);
   render(surf);
}

void renderSystem::render(surface& surf){

   GLScopedProgram winProgram(m_shaderProgramUI);
   GLScopedVAO tempVAO(m_quadVao);
   // Select texture unit 0 and bind colorTex to GL_TEXTURE_2D on that unit
   glActiveTexture(GL_TEXTURE0);
   GLScopedTexture2D tempTexture(surf.getSurface());

   glUniform1i(glGetUniformLocation(m_shaderProgramUI, "screenTexture"), 0);
   glDrawArrays(GL_TRIANGLES, 0, 6);
}

