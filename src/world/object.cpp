#include "object.hpp"
// GL Libraries
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "transform.hpp"

//---------------------- OBJECT TRANSFORMATIONS ----------------------

void object::scale(float sx, float sy, float sz){

   m_transform.scale *= glm::vec3(sx,sy,sz);
   m_body = m_model.generateMassProperties(m_transform.scale);
}

void object::move(float x, float y, float z){

   m_transform.position += glm::vec3(x, y, z);
   box.center = m_transform.position;
}

void object::rotate(float u, float v, float w){

   m_transform.rotateLocal(u, glm::vec3(1,0,0));
   m_transform.rotateLocal(v, glm::vec3(0,1,0));
   m_transform.rotateLocal(w, glm::vec3(0,0,-1));
}




