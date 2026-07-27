// Use GJK to detect collisions and EPA to determine the penatration..
// and normal forces (contact normals)
//
// GJK basically just finds the points closest to the projected contact point and
// decides if the points are overlapping in the 2 objects

#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "world/transform.hpp"


class collider{
public:
    virtual glm::vec3 support(const glm::vec3&) const = 0;
    virtual const glm::vec3& getCenter() const = 0;
    virtual ~collider() = default;
};

struct obb : public collider{
    glm::vec3 center;
    glm::vec3 axis[3];
    glm::vec3 halfExtents;

    const glm::vec3& getCenter() const override {return center; }
    glm::vec3 support(const glm::vec3& direction) const override;
};


class sphereCollider : public collider
{
public:
    sphereCollider(glm::vec3 center, float radius) : m_center(center), m_radius(radius){}

    glm::vec3 support(const glm::vec3& direction) const override{
        // Avoid normalizing a zero vector
        float length = glm::length(direction);

        if(length < 0.000001f) return m_center;
        return m_center + (direction / length) * m_radius;
    }

    const glm::vec3& getCenter() const override{
        return m_center;
    }

private:
    glm::vec3 m_center;
    float m_radius;
};

obb buildOBB(const transform& transform,const glm::vec3& localCenter,const glm::vec3& localHalfExtents);
