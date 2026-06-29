#pragma once
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// Standard Libraries
#include <vector>
// Project Headers



class surface{
public:

   surface(int w, int h){ initialize(w, h); }
   surface(){};
   ~surface();

   void size(int w, int h);
   glm::vec2 size() {return glm::vec2(m_width,m_height);}
   GLuint getFbo() const {return m_fbo;}
   GLuint getVao() const {return m_quadVao;}
   GLuint getSurface() const {return m_colorTex;}

private:

   void initialize(int w, int h);

   int m_height;
   int m_width;
   bool initialized = false;

   GLuint m_fbo = 0;
   GLuint m_colorTex = 0;
   GLuint m_depthTex = 0;

   GLuint m_quadVao;
   GLuint m_quadVbo;

   std::vector<float> m_quad;
};
