#include "surface.hpp"

// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Project headers
#include "render/RAIIWrapper.hpp"
#include <iostream>



void surface::initialize() {

   //---------------------- CREATE RENDER FBO ----------------------

   glGenFramebuffers(1, &m_fbo);
   GLScopedFBO tempFBO(m_fbo);
   {
      // Color attachment
      glGenTextures(1, &m_colorTex);
      GLScopedTexture2D tempTexture(m_colorTex);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      // Attach color texture
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTex, 0);
   }
   {
      glGenTextures(1, &m_depthTex);
      GLScopedTexture2D tempTexture(m_depthTex);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_size.x, m_size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      // Attach depth texture
      glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,m_depthTex,0);
   }
   GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
   glDrawBuffers(1, buffers);

   if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "Surface FBO incomplete\n";
}


void surface::resizeFbo(glm::ivec2 s){

   m_size = s;
   {
      GLScopedTexture2D tempTexture(m_colorTex);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
   }
   {
      GLScopedTexture2D tempDepth(m_depthTex);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_size.x, m_size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
   }
}


surface::~surface(){
   // Cleanup OpenGL objects
   if (m_quadVbo) glDeleteBuffers(1, &m_quadVbo);
   if (m_quadVao) glDeleteVertexArrays(1, &m_quadVao);
   if (m_colorTex) glDeleteTextures(1, &m_colorTex);
   if (m_depthTex) glDeleteTextures(1, &m_depthTex);
   if (m_fbo) glDeleteFramebuffers(1, &m_fbo);
}
