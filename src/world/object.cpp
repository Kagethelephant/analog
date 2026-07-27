#include "object.hpp"
// GL Libraries
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/RAIIWrapper.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "transform.hpp"
#include <iostream>

//---------------------- OBJECT TRANSFORMATIONS ----------------------

void object::scale(float sx, float sy, float sz){

    m_transform.scale *= glm::vec3(sx,sy,sz);
    m_body = m_model.generateMassProperties(m_transform.scale);
    updateObb();
    updateObbMesh();
}

void object::move(float x, float y, float z){

    m_transform.position += glm::vec3(x, y, z);
    updateObb();
    updateObbMesh();
}

void object::rotate(float u, float v, float w){

    m_transform.rotateLocal(u, glm::vec3(1,0,0));
    m_transform.rotateLocal(v, glm::vec3(0,1,0));
    m_transform.rotateLocal(w, glm::vec3(0,0,-1));
    updateObb();
    updateObbMesh();
}

void object::updateObb() {

    box.center = m_transform.position + m_transform.orientation * m_model.localCenter;
    box.halfExtents = m_model.localHalfExtents * m_transform.scale;

    box.axis[0] = m_transform.right();
    box.axis[1] = m_transform.up();
    box.axis[2] = m_transform.forward();

}


void object::buildObbMesh(){


    // Draw OBB
    glm::vec3 r = box.axis[0] * box.halfExtents.x;
    glm::vec3 u = box.axis[1] * box.halfExtents.y;
    glm::vec3 f = box.axis[2] * box.halfExtents.z;


    corners[0] = box.center - r - u - f;
    corners[1] = box.center + r - u - f;
    corners[2] = box.center + r + u - f;
    corners[3] = box.center - r + u - f;

    corners[4] = box.center - r - u + f;
    corners[5] = box.center + r - u + f;
    corners[6] = box.center + r + u + f;
    corners[7] = box.center - r + u + f;

    const uint32_t indices[] =
        {
            // bottom
            0,1,
            1,2,
            2,3,
            3,0,

            // top
            4,5,
            5,6,
            6,7,
            7,4,

            // vertical
            0,4,
            1,5,
            2,6,
            3,7
        };


    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    GLScopedVAO tempVAO(vao);

    GLScopedVBO tempVBO(vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(corners),corners,GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),(void*)0);

    glEnableVertexAttribArray(0);

    GLScopedEBO tempEBO(ebo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
}


void object::updateObbMesh(){

    glm::vec3 r = box.axis[0] * box.halfExtents.x;
    glm::vec3 u = box.axis[1] * box.halfExtents.y;
    glm::vec3 f = box.axis[2] * box.halfExtents.z;


    corners[0] = box.center - r - u - f;
    corners[1] = box.center + r - u - f;
    corners[2] = box.center + r + u - f;
    corners[3] = box.center - r + u - f;

    corners[4] = box.center - r - u + f;
    corners[5] = box.center + r - u + f;
    corners[6] = box.center + r + u + f;
    corners[7] = box.center - r + u + f;
    GLScopedVBO tempVBO(vbo);
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(corners),corners);
}

