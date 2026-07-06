#pragma once
// Program headers
#include "window/window.hpp"
#include "world/object.hpp"
#include "world/model.hpp"
#include "world/camera.hpp"
#include "shaders/shader.hpp"
#include "surface.hpp"
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Standard Libraries
#include <vector>




// Constant max number of lights (this needs to match 3d frag shader constant MAX_LIGHTS)
constexpr unsigned int MAX_LIGHTS = 32;

struct model3D{
   model3D(std::size_t i) : index(i){};
   std::size_t index;
};

class object3D;

/// @brief: GPU rendering engine to handle the OpenGL calls for 3D rendering for the provided
/// models / objects in 3D space. Draws to window FBO and window class handles rendering to GLFW window
class render3D {

public:

   /// @brief: Create new GPU rendering engine
   /// @param cam: Camera to reference for view matrix and window size
   render3D(surface& surf);

   /// @brief: Load object data to GPU and memory to render in loop with "render"
   /// @param obj: Object to load
   model3D loadModel(const std::string& filename, bool cwWinding = false);

   object3D createObject(model3D m);

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

   surface& getSurface(){return m_renderSurface;}

   const glm::vec2 getResolution() {return m_renderSurface.size();};

   /// @brief: Render the scene with loaded objects and lights to the window FBO
   void render(camera& cam);

   std::vector<object> objects;

private:
   
   /// @brief: OpenGL shader program comprised of vertex and fragment shaders pulled from the .glsl files
   GLuint m_shaderProgram3D;
   /// @brief: Shader program to render 2D quads with textures
   GLuint shaderProgramUI;


   std::vector<model> m_models;

   std::vector<float> m_lightPositions;
   std::vector<float> m_lightColors;

   surface& m_renderSurface;
};



class object3D{
public:
   object3D(render3D* m, std::size_t i) : parent(m), index(i) {};

   /// @brief: Changes object scale (absolute)
   /// @param sx: Scale in x
   /// @param sy: Scale in y
   /// @param sz: Scale in z
   void scale(float sx, float sy, float sz){parent->objects[index].scale(sx,sy,sz);};

   /// @brief: Changes object world coordinates (relative)
   /// @param x: x position
   /// @param y: y position
   /// @param z: z position
   void move(float x, float y, float z){parent->objects[index].move(x,y,z); };

   /// @brief: Changes object rotation (relative)
   /// @param u: rotation in radians about x axis
   /// @param v: rotation in radians about y axis
   /// @param w: rotation in radians about z axis
   void rotate(float u, float v, float w) {parent->objects[index].rotate(u,v,w);};

   /// @brief: Set color of object that will be drawn if the model does not have a texture
   /// @param _color: color as Color enum: 4 channel hexadecimal color
   void color(Color col) {parent->objects[index].color(col);};
   /// @brief: get object color as Color enum: 4 channel hexadecimal color
   Color color() const {return parent->objects[index].color();};


private:
   GLuint m_shaderProgram3D;
   render3D* parent;
   std::size_t index;
};



