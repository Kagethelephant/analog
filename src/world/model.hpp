
#pragma once
// Project Libraries
#include "utils/data.hpp"
// Standard Libraries
#include <vector>
#include <unordered_map>
#include <string>
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>



struct gpuSubMesh{
   bool textured;
   GLuint tex;
   GLuint ebo;
   std::size_t indiceCount;
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
   const GLuint& getVao() const { return vao; }
   const GLuint& getVbo() const { return vbo; }


private:

   /// @brief: Contains all sub meshes that make up the model
   std::vector<gpuSubMesh> subMeshes;

   /// @brief: Maps texture names from the .mtl file to loaded texture data.
   std::unordered_map<std::string, texture> m_textureMap;

   // Grab refernce to object on create so we can get things like object color during rendering
   GLuint vao;
   GLuint vbo;

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


