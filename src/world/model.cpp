#include "model.hpp"
// OpenGL
#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
// Standard Libraries
#include <iostream>





rigidBody model::generateMassProperties(const glm::vec3& scale) const {

   rigidBody body;
   float totalVolume = 0;
   glm::vec3 weightedCentroid(0.0f);

   float density = 1.0f;

   float totalxx(0.0f);
   float totalyy(0.0f);
   float totalzz(0.0f);
   float totalxy(0.0f);
   float totalxz(0.0f);
   float totalyz(0.0f);

   for(const subMesh& mesh: m_subMeshes) {
      for(int i = 0; i < mesh.indices.size()/3; i++ ){

         glm::vec3 A = m_vertices[mesh.indices[i*3]].position * scale;
         glm::vec3 B = m_vertices[mesh.indices[i*3 + 1]].position * scale;
         glm::vec3 C = m_vertices[mesh.indices[i*3 + 2]].position * scale;

         float volume = glm::dot(glm::cross(A, B), C) / 6.0f;
         glm::vec3 centroid = (A + B + C) * 0.25f;

         totalVolume += volume;
         weightedCentroid += centroid * volume;


         // Closed-form second moment integrals for a tetrahedron.
         totalxx += (volume / 10.0f) * (A.x*A.x + B.x*B.x + C.x*C.x + A.x*B.x + A.x*C.x + B.x*C.x);
         totalyy += (volume / 10.0f) * (A.y*A.y + B.y*B.y + C.y*C.y + A.y*B.y + A.y*C.y + B.y*C.y);
         totalzz += (volume / 10.0f) * (A.z*A.z + B.z*B.z + C.z*C.z + A.z*B.z + A.z*C.z + B.z*C.z);

         totalxy += (volume / 20.0f) * (2*A.x*A.y + 2*B.x*B.y + 2*C.x*C.y +
            A.x*B.y + B.x*A.y + 
            A.x*C.y + C.x*A.y + 
            B.x*C.y + C.x*B.y);

         totalxz += (volume / 20.0f) * (2*A.x*A.z + 2*B.x*B.z + 2*C.x*C.z +
            A.x*B.z + B.x*A.z + 
            A.x*C.z + C.x*A.z + 
            B.x*C.z + C.x*B.z);

         totalyz += (volume / 20.0f) * (2*A.y*A.z + 2*B.y*B.z + 2*C.y*C.z +
            A.y*B.z + B.y*A.z + 
            A.y*C.z + C.y*A.z + 
            B.y*C.z + C.y*B.z);
      }
   }

   // Construct inertia tensor
   // Calculates inertia with rotation about model origin
   glm::mat3 originInertia(0.0f);

   originInertia[0][0] = totalyy + totalzz;
   originInertia[1][0] = -totalxy;
   originInertia[2][0] = -totalxz;

   originInertia[0][1] = -totalxy;
   originInertia[1][1] = totalxx + totalzz;
   originInertia[2][1] = -totalyz;

   originInertia[0][2] = -totalxz;
   originInertia[1][2] = -totalyz;
   originInertia[2][2] = totalxx + totalyy;

   originInertia *= density;
   body.mass = totalVolume * density;

   // Parallel axis theorem
   if (std::abs(totalVolume) < 1e-6f)
    std::cout << "Mesh has zero volume." << std::endl;
   // Distance from origin to center of mass squared 
   body.com = weightedCentroid / totalVolume;
   glm::vec3 d = body.com;
   float d2 = glm::dot(d,d);


   // Calculate outer product
   glm::mat3 outer(0.0f);

   outer[0][0] = d.x * d.x;
   outer[1][0] = d.x * d.y;
   outer[2][0] = d.x * d.z;

   outer[0][1] = d.y * d.x;
   outer[1][1] = d.y * d.y;
   outer[2][1] = d.y * d.z;

   outer[0][2] = d.z * d.x;
   outer[1][2] = d.z * d.y;
   outer[2][2] = d.z * d.z;

   body.inertia = originInertia - ((d2 * glm::mat3(1.0f) - outer) * body.mass);
   
   return body;
}


