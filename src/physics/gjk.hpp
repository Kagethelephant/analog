#pragma once

#include "collider.hpp"
#include <array>
#include <glm/glm.hpp>



glm::vec3 support(const collider& a,const collider& b,glm::vec3 direction);

bool gjk(const collider& a,const collider& b);



struct simplex
{
    // Array of points in the simplex
    std::array<glm::vec3,4> points;
    int size = 0;

    
    void push_front(glm::vec3 point){
        // Move all points to the right 1 space
        for(int i=size; i>0; i--) {
            points[i] = points[i-1];
        }

        // Add new point to the front
        points[0] = point;
        if(size < 4) size++;
    }


    glm::vec3& operator[](int i){
        return points[i];
    }
};


bool updateSimplex(simplex& s, glm::vec3& direction);

bool line(simplex& s, glm::vec3& direction);
bool triangle(simplex& s, glm::vec3& direction);
bool tetrahedron(simplex& s, glm::vec3& direction);
