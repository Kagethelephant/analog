#include "collider.hpp"

#include "world/object.hpp"


glm::vec3 obb::support(const glm::vec3& direction) const{
    glm::vec3 point = center;

    // Build the support point one axis at a time.
    for(int i = 0; i < 3; i++){
        // Select the side of the box that faces the search direction.
        float sign = glm::dot(direction, axis[i]) >= 0 ? 1.0f : -1.0f;
        // Offset from the center to that face along this local axis, to get that component of the point.
        point += axis[i] * halfExtents[i] * sign;
    }

    return point;
}


obb buildOBB(const transform& transform,const glm::vec3& localCenter,const glm::vec3& localHalfExtents){
    obb box;

    box.center = transform.position + transform.orientation * localCenter;

    box.axis[0] = transform.right();
    box.axis[1] = transform.up();
    box.axis[2] = transform.forward();

    box.halfExtents = localHalfExtents * transform.scale;

    return box;
}


