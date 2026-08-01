#pragma once
// Standard Libraries
#include <vector>
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>



struct gpuSubMesh{
    bool textured;
    GLuint tex;
    GLuint ebo;
    std::size_t indiceCount;
};

struct vertex{
    glm::vec3 position;
    glm::vec2 uv;
};

struct rigidBody {
    float mass = 0.0f;
    glm::vec3 com = glm::vec3(0.0f,0.0f,0.0f);
    glm::mat3 inertia;
};
/// @brief: Stores texture data corisponding with a submesh
struct texture {
    int w;                           // Width of image
    int h;                           // Height of image
    int channels;                    // Number of channels (rgb = 3)
    unsigned char* data = nullptr;   // Pointer to begining of char array
};

/// @brief: Portion of mesh associated with a single texture
struct subMesh {
    std::vector<uint32_t> indices;   // Indices making up submesh (triangles)
    bool textured = true;            // Was a texture loaded for the mesh
    texture tex;                     // Texture instance storing image data
};




//---------------------- MODEL ----------------------

/// @brief: This stores all of the vertex data and attributes loaded from the obj file and is 
/// referenced by an object so we dont have to load multiple models for objects with the same model
class model {

public:

    /// @brief: Loads model from obj file
    /// @param filename: filepath to the OBJ file
    /// @param ccwWinding: changes the winding on the model so the triangle normal points outwards
    // model(const std::string& filename, bool cwWinding);
    model(std::vector<gpuSubMesh> m, std::vector<subMesh> m2, std::vector<vertex> v, GLuint vao, GLuint vbo):
        subMeshes(m), m_subMeshes(m2), m_vertices(v), m_vao(vao), m_vbo(vbo){
        m_body = generateMassProperties(glm::vec3(1.0f));
    };



    /// @brief: Get submesh vector array
    const std::vector<gpuSubMesh>& getSubMeshes() const { return subMeshes; }
    const GLuint& getVao() const { return m_vao; }
    const GLuint& getVbo() const { return m_vbo; }

    const rigidBody& getMassProperties() const {return m_body;}


    rigidBody generateMassProperties(const glm::vec3& scale) const;

    /// @brief: half the width, height and length of object to make obb
    glm::vec3 localHalfExtents;
    /// @brief: Center of the bounding box, not the origin or center of mass
    glm::vec3 localCenter;
    float radius;


private:

    rigidBody m_body;

    /// @brief: Contains all sub meshes that make up the model
    std::vector<gpuSubMesh> subMeshes;

    // Grab refernce to object on create so we can get things like object color during rendering
    GLuint m_vao;
    GLuint m_vbo;

    /// @brief: Contains all sub meshes that make up the model
    std::vector<subMesh> m_subMeshes;
    /// @brief: Tightly packed vertex data for GPU rendering
    std::vector<vertex> m_vertices;
};


