#pragma once
// Project Libraries
#include "glm/fwd.hpp"
#include "utils/data.hpp"
#include "model.hpp"
#include "physics/collider.hpp"
// Standard Libraries

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "transform.hpp"






class light{
public:

   void setColor(float r, float g, float b){color = glm::vec4(r,g,b,1.0f);} 
   glm::vec4 getColor() {return color;};

   transform t;

private:
   glm::vec4 color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
};


class gpuLight{
public:

   gpuLight(glm::vec3 p,glm::vec4 c ): position(glm::vec4(p,1.0f)), color(glm::vec4(c)){};
   glm::vec4 position;
   glm::vec4 color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
};


//---------------------- OBJECT ----------------------

/// @brief: Object that can instantiate models and position them in world space
class object {

public:
 
   /// @brief: Create object with model reference for geometry
   /// @param model: Model object references for geometry
   object(model m) : m_model{m} {
      glm::mat4 m_modelMatrix = glm::mat4(1.0f);
   };


   /// @brief: Changes object scale (absolute)
   /// @param sx: Scale in x
   /// @param sy: Scale in y
   /// @param sz: Scale in z
   void scale(float sx, float sy, float sz);

   /// @brief: Changes object world coordinates (relative)
   /// @param x: x position
   /// @param y: y position
   /// @param z: z position
   void move(float x, float y, float z);

   /// @brief: Changes object rotation (relative)
   /// @param u: rotation in radians about x axis
   /// @param v: rotation in radians about y axis
   /// @param w: rotation in radians about z axis
   void rotate(float u, float v, float w);


   /// @brief: Set color of object that will be drawn if the model does not have a texture
   /// @param _color: color as Color enum: 4 channel hexadecimal color
   void color(Color col) {m_color = col;};
   /// @brief: get object color as Color enum: 4 channel hexadecimal color
   Color color() const {return (m_color);};
   /// @brief: get object transformation matrix
   const glm::mat4 getModelMatrix() const {return (m_transform.matrix());};

   /// @brief: Get model referenced by this object
   const model getModel() const {return m_model;};
   const rigidBody getBody() const {return m_body;};

   obb box;

private:

   /// @brief: Base color to draw the object (this will be shaded by the camera)
   Color m_color = Color::White;


   /// @brief: model matrix used in the vertex shader of the rendering pipeline
   glm::mat4 m_modelMatrix;

   /// @brief: Reference to object model (geometry and texture data)
   model m_model;
   rigidBody m_body;

   transform m_transform;
};


