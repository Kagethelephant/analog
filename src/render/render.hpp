#pragma once
// Program headers
#include "window/window.hpp"
#include "world/object.hpp"
#include "world/camera.hpp"
#include "shaders/shader.hpp"
#include "surface.hpp"
#include "window/text.hpp"
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Standard Libraries
#include <vector>



// Constant max number of lights (this needs to match 3d frag shader constant MAX_LIGHTS)
constexpr unsigned int MAX_LIGHTS = 32;


/// @brief: GPU rendering engine to handle the OpenGL calls for 3D rendering for the provided
/// models / objects in 3D space. Draws to window FBO and window class handles rendering to GLFW window
class gpuRenderEngine {

public:

   /// @brief: Create new GPU rendering engine
   /// @param cam: Camera to reference for view matrix and window size
   gpuRenderEngine(camera& cam, const window& win);

   /// @brief: Load object data to GPU and memory to render in loop with "render"
   /// @param obj: Object to load
   void bindObject(const object& obj);

   /// @brief: Load lights into engine to use for rendering
   /// @param newLight: Light to be loaded
   void addLight(const light& newLight){
      m_lightPositions.push_back(newLight.position.x);
      m_lightPositions.push_back(newLight.position.y);
      m_lightPositions.push_back(newLight.position.z);
      m_lightColors.push_back(newLight.color.x);
      m_lightColors.push_back(newLight.color.y);
      m_lightColors.push_back(newLight.color.z);
   }

   surface& getActiveSurface(){return renderSurface;}

   const glm::vec2 getResolution() {return renderSurface.size();};

   /// @brief: Render the scene with loaded objects and lights to the window FBO
   void render();

   void drawText(std::string text, float x, float y, Color col = Color::White){

      textRend.RenderText(renderSurface, text, x, y, col);
   }

   void draw();

private:
   
   /// @brief: OpenGL shader program comprised of vertex and fragment shaders pulled from the .glsl files
   GLuint m_shaderProgram3D;
   /// @brief: Shader program to render 2D quads with textures
   GLuint shaderProgramUI;

   /// @brief: Sub mesh parsed from object submeshes on load. Meshes are uploaded to GPU one time to reduce overhead.
   /// This struct groups the gpu data like mesh textures and ebo (indices) to reference during rendering
   struct gpuSubMesh{
      bool textured;
      GLuint tex;
      GLuint ebo;
      std::size_t indiceCount;
   };

   /// @brief: Object parsed from "bindObject" to load into the GPU one time to reduce overhead.
   /// This struct groups the gpu data like vao, vbo and submeshes for reference during rendering
   struct gpuObject {
      // Grab refernce to object on create so we can get things like object color during rendering
      gpuObject(const object& o) : obj{o} {}
      const object& obj;
      GLuint vao;
      GLuint vbo;
      std::vector<gpuSubMesh> subMeshes;
   };

   std::vector<gpuObject> m_objects;

   std::vector<float> m_lightPositions;
   std::vector<float> m_lightColors;

   const window& m_window;
   camera& m_camera;

   surface renderSurface;

   GLuint m_quadVao;
   GLuint m_quadVbo;

   textRenderEngine textRend = textRenderEngine("../resources/font/novem___.ttf");
   std::vector<float> m_quad;

};

