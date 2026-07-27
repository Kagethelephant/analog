#include "gjk.hpp"
#include "utils/debug.hpp"
#include <iostream>

glm::vec3 support(const collider& a,const collider& b,glm::vec3 direction){
    return a.support(direction) - b.support(-direction);
}

bool gjk(const collider& a,const collider& b){

    // Initial direction
    glm::vec3 direction = a.getCenter() - b.getCenter();

    simplex points;

    // Insert the support point for the initial direction
    points.push_front(support(a,b,direction));

    // Look towards the origin from original point
    direction = -points[0];

    // Safety feature for infinate loop
    int itterations = 0;

    while(true)
    {
        // Safety feature for infinate loop
        itterations ++;
        if (itterations > 10){
            std::cout << "GJK failed to converge" << std::endl;
            return false;
        }

        // Find a new support point with the oposite direction of the intial
        glm::vec3 newPoint = support(a,b,direction);


        // Make sure this point has not been checked (not making progress towards convergance)
        for(int i=0;i<points.size;i++){
            if(glm::length(newPoint - points[i]) < 0.00001f){
                // std::cout << "No progress\n";
                return false;
            }
        }

        // Since this is a convex shape, the point will be on same side of the origin as the direction if the origin is in the simplex
        if(glm::dot(newPoint,direction) <= 0) return false;

        // If we have not early exited yet then this is a valid point
        points.push_front(newPoint);

        // attempt to build the rest of the simplex. Will return true if tet was created and origin is within the tet
        if(updateSimplex(points,direction)) return true;
    }
}

bool updateSimplex(simplex& s, glm::vec3& direction){
    // Depending on the qty of points in the simplex use different function to set new direction
    switch(s.size){
        case 2: return line(s,direction);
        case 3: return triangle(s,direction);
        case 4: return tetrahedron(s,direction);
    }
    // Return false if there is only one point (invalid simplex)
    return false;
}


bool line(simplex& s, glm::vec3& direction){
    glm::vec3 A = s[0];
    glm::vec3 B = s[1];

    // A pointing at Origin
    glm::vec3 AO = -A;
    // A pointing at B
    glm::vec3 AB = B-A;


    // Asking if the new point A passed the origin
    if(glm::dot(AB,AO) > 0){
        // triple product cross(cross(AB,AC),BC) returns vector perpindicular to BC on the plane shared by AB and AC
        direction = glm::cross(glm::cross(AB,AO),AB);
    }
    // If B is not any closer, remove it by reducing the size
    else{
        // B is at the back of the array so reducing the size effectively removes it
        s.size = 1;
        direction = AO;
    }


    return false;
}




bool triangle(simplex& s, glm::vec3& direction){
    glm::vec3 A=s[0];
    glm::vec3 B=s[1];
    glm::vec3 C=s[2];

    // A pointing at origin
    glm::vec3 AO = -A;

    // A pointing at B and C
    glm::vec3 AB = B-A;
    glm::vec3 AC = C-A;


    glm::vec3 ABC = glm::cross(AC,AB);


    

    // !!!!! THESE EDGE CHECKS ARE REVERSED AND INCORRECT !!!!!

    // Check AB side
    // Triple product giving vector perpindicular to AB on the plane ABC
    glm::vec3 abPerp = glm::cross(ABC,AB);
    if(glm::dot(abPerp,AO) > 0){
        // C is already at thte back of the array so reducing the "size" effectively removes it
        s.size=2;
        return line(s,direction);
    }


    // Check AC side
    // Triple product giving vector perpindicular to AC on the plane ABC
    glm::vec3 acPerp =glm::cross(AC,ABC);
    if(glm::dot(acPerp,AO)>0){
        s.points[1]=s.points[2];
        s.size=2;
        return line(s,direction);
    }

    // BC never needs to be tested, this was tested when A was added in the previous itteration

    // Origin is above or below triangle
    if(glm::dot(ABC,AO)>0){
        direction=ABC;
    }
    else{
        // Reverse winding
        glm::vec3 temp=s.points[1];
        s.points[1]=s.points[2];
        s.points[2]=temp;

        direction=-ABC;
    }


    return false;
}


bool tetrahedron(simplex& s, glm::vec3& direction){
    glm::vec3 A=s[0];
    glm::vec3 B=s[1];
    glm::vec3 C=s[2];
    glm::vec3 D=s[3];


    // A pointing at origin
    glm::vec3 AO=-A;


    // A pointing at B and C
    glm::vec3 AB = B-A;
    glm::vec3 AC = C-A;
    glm::vec3 AD = D-A;


    glm::vec3 ABC = glm::cross(AC,AB);
    glm::vec3 ACD = glm::cross(AD,AC);
    glm::vec3 ADB = glm::cross(AB,AD);

    // ABC face
    // glm::vec3 ABC = glm::cross(B-A,C-A);
    if(glm::dot(ABC,AO)>0){
        s.size=3;
        return triangle(s,direction);
    }


    // ACD face
    // glm::vec3 ACD = glm::cross(C-A,D-A);
    if(glm::dot(ACD,AO)>0){
        s.points[1]=C;
        s.points[2]=D;
        s.size=3;

        return triangle(s,direction);
    }


    // ADB face
    // glm::vec3 ADB = glm::cross(D-A,B-A);
    if(glm::dot(ADB,AO)>0){
        s.points[1]=D;
        s.points[2]=B;
        s.size=3;

        return triangle(s,direction);
    }

    // Origin is inside tetrahedron
    return true;
}
