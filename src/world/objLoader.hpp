#pragma once
// Standard Libraries
#include <vector>
#include <unordered_map>
#include <string>
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "model.hpp"

#include <glm/glm.hpp>








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
std::unordered_map<std::string, texture> loadMTL(const std::string& filepath);

/// @brief: Gets the directory that the given file is located
/// @param filename: path to file
std::string getDirectory(const std::string& filepath);

model loadModel(const std::string& filename, bool cwWinding);

