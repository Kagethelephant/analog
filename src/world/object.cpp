#include "object.hpp"
// GL Libraries
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "iostream"
#include "utils/debug.hpp"



//---------------------- OBJECT TRANSFORMATIONS ----------------------

void object::scale(float sx, float sy, float sz){
   m_scale = glm::vec3(sx, sy, sz);
   updateModelMatrix();
   m_body = m_model.generateMassProperties(m_scale);

   // std::cout << std::endl;
   // print(m_body.inertia);
   // std::cout << "COM: (" << m_body.com.x << ", "<< m_body.com.y << ", "<< m_body.com.z << ")" << std::endl;

}
void object::move(float x, float y, float z){
   m_position += glm::vec3(x, y, z);
   updateModelMatrix();
}
void object::rotate(float u, float v, float w){
   m_rotation += glm::vec3(u, v, w);
   updateModelMatrix();
}

void object::updateModelMatrix()
{
   glm::mat4 T = glm::translate(glm::mat4(1.0f), m_position);

   glm::mat4 R(1.0f);
   R = glm::rotate(R, m_rotation.x, glm::vec3(1,0,0));
   R = glm::rotate(R, m_rotation.y, glm::vec3(0,1,0));
   R = glm::rotate(R, m_rotation.z, glm::vec3(0,0,1));

   glm::mat4 S = glm::scale(glm::mat4(1.0f), m_scale);

   m_modelMatrix = T * R * S;
}



