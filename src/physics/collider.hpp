// Use GJK to detect collisions and EPA to determine the penatration..
// and normal forces (contact normals)
//
// GJK basically just finds the points closest to the projected contact point and
// decides if the points are overlapping in the 2 objects

#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "world/transform.hpp"

struct obb {
   glm::vec3 center;
   glm::vec3 axis[3];
   glm::vec3 halfExtents;
};

obb buildOBB(const transform& transform,const glm::vec3& localCenter,const glm::vec3& localHalfExtents);
