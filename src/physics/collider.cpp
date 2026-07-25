#include "collider.hpp"

#include "world/object.hpp"


obb buildOBB(const transform& transform,const glm::vec3& localCenter,const glm::vec3& localHalfExtents){
    obb box;

    box.center = transform.position + transform.orientation * localCenter;

    box.axis[0] = transform.right();
    box.axis[1] = transform.up();
    box.axis[2] = transform.forward();

    box.halfExtents = localHalfExtents * transform.scale;

    return box;
}
