#include "transform.hpp"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



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
