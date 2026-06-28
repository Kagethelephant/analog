#pragma once
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// Standard Libraries
#include <vector>
// Project Headers
#include "utils/matrix.hpp"



class surface{
public:

   surface(int w, int h);
   ~surface();

   void size(int w, int h);
   glm::vec2 size() {return glm::vec2(m_width,m_height);}
   GLuint getFbo() const {return m_fbo;}
   GLuint getVao() const {return m_quadVao;}
   GLuint getSurface() const {return m_colorTex;}

private:

   int m_height;
   int m_width;

   GLuint m_fbo = 0;
   GLuint m_colorTex = 0;
   GLuint m_depthTex = 0;

   GLuint m_quadVao;
   GLuint m_quadVbo;

   std::vector<float> m_quad;
};
