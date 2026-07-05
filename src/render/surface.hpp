#pragma once
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// Standard Libraries
#include <vector>
#include "window/window.hpp"
// Project Headers



class surface : public IResizeTarget {
public:

   enum class resizeMode{
      fixedWidth,
      fixedHeight
   };

   // Constructors for different surface resize types
   surface(int w, int h){ initialize(); fixed(w,h); }
   surface(const IResizeTarget& w){ initialize(); match(w); }
   surface(const IResizeTarget& w, int s, resizeMode m){ 
      initialize(); 
      if(m == resizeMode::fixedWidth) {
         matchFixedWidth(w,s);
      }
      else{matchFixedHeight(w,s);
      } 
   }

   ~surface();

   glm::ivec2 size() const override{return m_size;}
   GLuint getFbo() const {return m_fbo;}
   GLuint getVao() const {return m_quadVao;}
   GLuint getSurface() const {return m_colorTex;}
   float getAspect() const {return static_cast<float>(m_size.x) / m_size.y;}


   // Functions for changing the resize mode
   void fixed(int w, int h){setResizeFunction([=](){return glm::ivec2{w, h};}); resize();}
   void match(const IResizeTarget& surf){setResizeFunction([&surf](){return surf.size();}); resize();}

   void matchFixedHeight(const IResizeTarget& surf, int h){
      
      setResizeFunction([&surf, h](){
         auto s = surf.size(); 
         float aspect = static_cast<float>(s.x) / s.y;
         return glm::ivec2{h * aspect,h};
      }); 
      resize();
   }
   void matchFixedWidth(const IResizeTarget& surf, int w){
      setResizeFunction([&surf, w](){
         auto s = surf.size();
         float aspect = static_cast<float>(s.x) / s.y;
         return glm::ivec2{w,w/ aspect};});
      resize();
   }


   using ResizeFunction = std::function<glm::ivec2()>;
   void setResizeFunction(ResizeFunction fn){ m_resizeCallback = std::move(fn); }
   void resize(){ if (!m_resizeCallback) return; m_resizeFbo(m_resizeCallback()); }

private:

   ResizeFunction m_resizeCallback;

   void initialize();
   void m_resizeFbo(glm::ivec2 s);

   glm::ivec2 m_size = glm::ivec2(2,2);

   GLuint m_fbo = 0;
   GLuint m_colorTex = 0;
   GLuint m_depthTex = 0;

   GLuint m_quadVao;
   GLuint m_quadVbo;

};
