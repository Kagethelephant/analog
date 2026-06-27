#include "camera.hpp"
// OpenGL
#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
// Standard Libraries
#include <math.h>
#include "utils/matrix.hpp"




//---------------------- CAMERA MOVEMENT ----------------------
void camera::move(float x, float y, float z) {
   // Transform the world-space up vector by the camera's rotation
   vec3 up = (matrix_rotate(m_rotation[0], m_rotation[1], m_rotation[2]) * vec4(0,1,0,1)).xyz();
   // Move along camera's right vector
   // use cross product of up and forward vector to get the the right vector
   m_position += (m_direction.cross(up) * x);
   // Movement in z and y is the same: not world axis but relative to camera direction 
   m_position += m_direction * z;
   m_position += up * y;

   // Updated view matrix will be referenced by renderer
   m_viewMatrix = matrix_view(matrix_pointAt(m_position, m_direction, up));
}

void camera::rotate(float u, float v, float w) {
   m_rotation += vec3(u, v, w);
   vec3 up = (matrix_rotate(m_rotation[0], m_rotation[1], m_rotation[2]) * vec4(0,1,0,1)).xyz();
   m_direction = (matrix_rotate(m_rotation[0], m_rotation[1],m_rotation[2]) * vec4(0,0,-1,1)).xyz() ;
   // Updated view matrix will be referenced by renderer
   m_viewMatrix = matrix_view(matrix_pointAt(m_position, m_direction, up));
}

void camera::updateView(){
   if (m_aspectRatio != m_window.getAspectRatio()){
      m_aspectRatio = m_window.getAspectRatio();
      m_projectionMatrix = matrix_project(m_fov, m_aspectRatio,m_near,m_far);  
   }
}
