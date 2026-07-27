#include "camera.hpp"
// OpenGL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include "transform.hpp"
// Standard Libraries




//---------------------- CAMERA MOVEMENT ----------------------
void camera::move(float x, float y, float z){

    if (!(x == 0.0f && y == 0.0f && z == 0.0f)){

        m_transform.translate(m_transform.right()*x + m_transform.up()*y + m_transform.forward()*z);
        m_viewMatrix = glm::lookAt( m_transform.position, m_transform.position + m_transform.forward(), m_transform.up());
    }
}

void camera::rotate(float pitch, float yaw, float roll)
{
    if (!(pitch == 0.0f && yaw == 0.0f && roll == 0.0f)){
        m_transform.rotateLocal(pitch, glm::vec3(1,0,0));
        m_transform.rotateWorld(yaw, glm::vec3(0,1,0));
        m_transform.rotateLocal(roll, glm::vec3(0,0,-1));

        m_viewMatrix = glm::lookAt( m_transform.position, m_transform.position + m_transform.forward(), m_transform.up());
    }

}

