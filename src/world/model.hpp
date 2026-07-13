
#pragma once
// Standard Libraries
#include <vector>
#include <unordered_map>
#include <string>
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtc/quaternion.hpp>



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

//---------------------- MODEL ----------------------

/// @brief: This stores all of the vertex data and attributes loaded from the obj file and is 
/// referenced by an object so we dont have to load multiple models for objects with the same model
class model {

public:

   /// @brief: Loads model from obj file
   /// @param filename: filepath to the OBJ file
   /// @param ccwWinding: changes the winding on the model so the triangle normal points outwards
   model(const std::string& filename, bool cwWinding);

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


   /// @brief: Get submesh vector array
   const std::vector<gpuSubMesh>& getSubMeshes() const { return subMeshes; }
   const GLuint& getVao() const { return m_vao; }
   const GLuint& getVbo() const { return m_vbo; }

   const rigidBody& getMassProperties() const {return m_body;}


   rigidBody generateMassProperties(const glm::vec3& scale) const;

private:

   rigidBody m_body;

   /// @brief: Contains all sub meshes that make up the model
   std::vector<gpuSubMesh> subMeshes;

   /// @brief: Maps texture names from the .mtl file to loaded texture data.
   std::unordered_map<std::string, texture> m_textureMap;

   // Grab refernce to object on create so we can get things like object color during rendering
   GLuint m_vao;
   GLuint m_vbo;

   /// @brief: Represents a unique combination of a vertex position index (v) and texture index (t)
   /// for use as a key in unordered_map. operator== is used to compare keys that hash to the same bucket.
   struct vertexKey {
      int v;
      int t;
      bool operator==(const vertexKey& o) const {
         return v == o.v && t == o.t;
      }
   };


   /// @brief Hash functor for vertexKey used in std::unordered_map.
   /// Combines position index (v) and texture index (t) into one hash value. Each integer is hashed 
   /// with std::hash<int>(). The texture hash is shifted left by one bit (<<1) so its bits occupy 
   /// different positions, then both hashes are mixed using XOR (^).
   ///
   /// Example:
   /// hv = 10101100
   /// ht = 00110101 -> (ht<<1) = 01101010
   /// result = hv ^ (ht<<1) = 11000110 (xor operator)
   ///
   /// This improves distribution across buckets. Collisions are resolved by vertexKey::operator==.
   struct vertexKeyHash {
      size_t operator()(const vertexKey& k) const {
         return std::hash<int>()(k.v) ^ (std::hash<int>()(k.t) << 1);
      }
   };

   /// @brief: Contains all sub meshes that make up the model
   std::vector<subMesh> m_subMeshes;
   /// @brief: Tightly packed vertex data for GPU rendering
   std::vector<vertex> m_vertices;

   /// @brief: load image data into a texture object from file
   /// @param filename: filepath of texture
   texture loadTexture(const std::string& filepath);

   /// @brief: Map all textures to names in the MTL file
   /// @param filename: filepath of mtl file
   void loadMTL(const std::string& filepath);

   /// @brief: Gets the directory that the given file is located
   /// @param filename: path to file
   std::string getDirectory(const std::string& filepath);

};


