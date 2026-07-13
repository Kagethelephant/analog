#include "camera.hpp"
// OpenGL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Standard Libraries




//---------------------- CAMERA MOVEMENT ----------------------
void camera::move(float x, float y, float z){
   m_position += m_right * x;
   m_position += m_up * y;
   m_position += m_forward * z;
   updateView();
}

void camera::rotate(float u, float v, float w){
   m_rotation += glm::vec3(u, v, w);
   updateView();
}


void camera::updateView()
{

   glm::mat4 rotation(1.0f);

   rotation = glm::rotate(rotation,m_rotation.y,glm::vec3(0.0f,1.0f,0.0f));
   rotation = glm::rotate(rotation,m_rotation.x,glm::vec3(1.0f,0.0f,0.0f));
   rotation = glm::rotate(rotation,m_rotation.z,glm::vec3(0.0f,0.0f,1.0f));

   m_forward = glm::normalize(glm::vec3(rotation * glm::vec4(0,0,-1,0)));
   m_right = glm::normalize(glm::cross(m_forward, glm::vec3(0,1,0)));
   m_up = glm::normalize(glm::cross(m_right, m_forward));

   m_viewMatrix = glm::lookAt(m_position,m_position + m_forward,m_up);
}

