#pragma once
// Project Libraries
#include "utils/data.hpp"
#include "utils/matrix.hpp"
// Standard Libraries
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>



//---------------------- LIGHTS ----------------------

/// @brief: Point light used for scene shading
struct light {
   /// @brief: Create light with starting pos and color
   /// @param pos: position of the light in world space
   /// @param col: RGB light color/intensity (0–1 range)
   light (const vec3& pos, const vec3& col = vec3(1,1,1)) : position{pos}, color{col} {}
   vec3 position;
   vec3 color;
};



//---------------------- MODEL ----------------------

/// @brief: This stores all of the vertex data and attributes loaded from the obj file and is 
/// referenced by an object so we dont have to load multiple models for objects with the same model
class model {

public:

   /// @brief: Loads model from obj file
   /// @param filename: filepath to the OBJ file
   /// @param ccwWinding: changes the winding on the model so the triangle normal points outwards
   model(const std::string& filename, bool cwWinding = false);

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
   const std::vector<subMesh>& getSubMeshes() const { return m_subMeshes; }
   /// @brief: Get raw vertice vector array
   const std::vector<float>& getVertices() const { return m_vertices; }


private:

   /// @brief: Contains all sub meshes that make up the model
   std::vector<subMesh> m_subMeshes;
   /// @brief: Tightly packed vertex data for GPU rendering
   std::vector<float> m_vertices;

   /// @brief: Maps texture names from the .mtl file to loaded texture data.
   std::unordered_map<std::string, texture> m_textureMap;

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



//---------------------- OBJECT ----------------------

/// @brief: Object that can instantiate models and position them in world space
class object {

public:
 
   /// @brief: Create object with model reference for geometry
   /// @param model: Model object references for geometry
   object(model& model) : m_model{model} {
      m_position = glm::vec3(0,0,0);
      m_rotation = glm::vec3(0,0,0);
      m_scale = glm::vec3(1,1,1);
      glm::mat4 m_modelMatrix = glm::mat4(1.0f);
   };


   /// @brief: Changes object scale (absolute)
   /// @param sx: Scale in x
   /// @param sy: Scale in y
   /// @param sz: Scale in z
   void scale(float sx, float sy, float sz);

   /// @brief: Changes object world coordinates (relative)
   /// @param x: x position
   /// @param y: y position
   /// @param z: z position
   void move(float x, float y, float z);

   /// @brief: Changes object rotation (relative)
   /// @param u: rotation in radians about x axis
   /// @param v: rotation in radians about y axis
   /// @param w: rotation in radians about z axis
   void rotate(float u, float v, float w);

   void updateModelMatrix();

   /// @brief: Set color of object that will be drawn if the model does not have a texture
   /// @param _color: color as Color enum: 4 channel hexadecimal color
   void color(Color col) {m_color = col;};
   /// @brief: get object color as Color enum: 4 channel hexadecimal color
   Color color() const {return (m_color);};
   /// @brief: get object transformation matrix
   const glm::mat4& getModelMatrix() const {return (m_modelMatrix);};
   /// @brief: Get model referenced by this object
   const model& getModel() const {return (m_model);};

private:

   /// @brief: Base color to draw the object (this will be shaded by the camera)
   Color m_color = Color::White;
   /// @brief: Coordinates of the object origin in 3D space
   glm::vec3 m_position = glm::vec3(0,0,0);
   /// @brief: Rotation from original orientation in radians
   glm::vec3 m_rotation = glm::vec3(0,0,0);
   /// @brief: Scale of the object
   glm::vec3 m_scale = glm::vec3(1,1,1);

   /// @brief: model matrix used in the vertex shader of the rendering pipeline
   glm::mat4 m_modelMatrix;

   /// @brief: Reference to object model (geometry and texture data)
   model& m_model;
};


