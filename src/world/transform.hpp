#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class transform{
public:
    glm::vec3 position = glm::vec3(0);
    glm::quat orientation = glm::quat(1,0,0,0);
    glm::vec3 scale = glm::vec3(1,1,1);

    void translate(glm::vec3 delta);
    void scaleBy(glm::vec3 delta);

    void rotateLocal(float degrees, const glm::vec3& axis);

    void rotateWorld(float degrees, const glm::vec3& axis);

    glm::mat4 matrix() const;

    glm::vec3 forward() const {return glm::normalize(orientation * glm::vec3(0,0,-1)); };
    glm::vec3 right() const {return glm::normalize(orientation * glm::vec3(1,0,0)); };
    glm::vec3 up() const {return glm::normalize(orientation * glm::vec3(0,1,0)); };
};
