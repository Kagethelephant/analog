#include "object.hpp"
// GL Libraries
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "iostream"
#include "utils/debug.hpp"



void transform::translate(glm::vec3 delta){
   position += delta;
}

void transform::scaleBy(glm::vec3 delta){
   scale *= delta;
}

void transform::rotateWorld(float degrees, const glm::vec3& axis){

    glm::quat q = glm::angleAxis( glm::radians(degrees), glm::normalize(axis));
    orientation = glm::normalize(q * orientation);
}


void transform::rotateLocal(float degrees, const glm::vec3& axis){

    glm::quat q = glm::angleAxis( glm::radians(degrees), glm::normalize(axis));
    orientation = glm::normalize(orientation * q);
}

glm::mat4 transform::matrix() const
{
    glm::mat4 translation = glm::translate(glm::mat4(1.0f),position);
    glm::mat4 rotation = glm::mat4_cast(orientation);
    glm::mat4 scaling = glm::scale(glm::mat4(1.0f),scale);

    return translation * rotation * scaling;
}


//---------------------- OBJECT TRANSFORMATIONS ----------------------

void object::scale(float sx, float sy, float sz){
   // m_scale = glm::vec3(sx, sy, sz);
   // updateModelMatrix();
   m_transform.scale *= glm::vec3(sx,sy,sz);
   m_body = m_model.generateMassProperties(m_transform.scale);

   // std::cout << std::endl;
   // print(m_body.inertia);
   // std::cout << "COM: (" << m_body.com.x << ", "<< m_body.com.y << ", "<< m_body.com.z << ")" << std::endl;

}
void object::move(float x, float y, float z){
   m_transform.position += glm::vec3(x, y, z);
   // updateModelMatrix();
}
void object::rotate(float u, float v, float w){
   m_transform.rotateLocal(u, glm::vec3(1,0,0));
   m_transform.rotateLocal(v, glm::vec3(0,1,0));
   m_transform.rotateLocal(w, glm::vec3(0,0,-1));
   // m_rotation += glm::vec3(u, v, w);
   // updateModelMatrix();
}

// void object::updateModelMatrix()
// {
//    glm::mat4 T = glm::translate(glm::mat4(1.0f), m_position);

//    glm::mat4 R(1.0f);
//    R = glm::rotate(R, m_rotation.x, glm::vec3(1,0,0));
//    R = glm::rotate(R, m_rotation.y, glm::vec3(0,1,0));
//    R = glm::rotate(R, m_rotation.z, glm::vec3(0,0,1));

//    glm::mat4 S = glm::scale(glm::mat4(1.0f), m_scale);

//    m_modelMatrix = T * R * S;
// }



