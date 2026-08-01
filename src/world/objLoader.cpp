#include "objLoader.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "model.hpp"
#include "render/RAIIWrapper.hpp"
#include "utils/debug.hpp"
// OpenGL
#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
// Standard Libraries
#include <math.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <algorithm>
// STB_Image
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


#include <iostream>



//---------------------- LOAD MODEL FROM OBJ FILE ----------------------

model loadModel(const std::string& filename, bool cwWinding) {

    std::unordered_map<std::string, texture> textureMap;
    rigidBody body;
    /// @brief: Contains all sub meshes that make up the model
    std::vector<gpuSubMesh> subMeshes;
    /// @brief: Contains all sub meshes that make up the model
    std::vector<subMesh> vertSubMeshes;
    /// @brief: Tightly packed vertex data for GPU rendering
    std::vector<vertex> vertices;

    GLuint vao;
    GLuint vbo;


    std::ifstream obj(filename);
    if (!obj) { throw std::runtime_error("Failed to open OBJ file");}
    std::string dir = getDirectory(filename);

    // Raw OBJ attribute streams
    std::vector<glm::vec3> objPositions;
    std::vector<glm::vec2> objTexcoords;


    // hash table mapping a combination of position and texture indexes to a vertex object 
    // in the vertex vector arry (vertices) vertexKeyHash is the callable struct to generate hash
    std::unordered_map<vertexKey, uint32_t, vertexKeyHash> vertexCache;

    subMesh* currentSubmesh;

    std::string line;
    while (std::getline(obj, line)) {

        std::stringstream stream(line);
        std::string type;
        stream >> type;

        // ---------- POSITION ----------
        if (type == "v") {
            glm::vec3 p;
            stream >> p.x >> p.y >> p.z;
            objPositions.push_back(p);
        }

        // ---------- TEXCOORD ----------
        else if (type == "vt") {
            glm::vec2 uv;
            stream >> uv.x >> uv.y;
            objTexcoords.push_back(uv);
        }

        // ---------- MTL FILE ----------
        else if (type == "mtllib") {
            std::string mtlfile;
            stream >> mtlfile;
            // Maps "mtlName" below to its corresponding texture data in "textureMap"
            textureMap = loadMTL(dir + mtlfile);
        }

        // ---------- MTL MATERIAL ----------
        else if (type == "usemtl") {
            std::string mtlName;
            stream >> mtlName;
            // loadMTL should already be called at this point so we just need to create a new
            // submesh and tie the texture associated with this mtlName to that submesh
            subMesh newSubmesh;
            newSubmesh.tex = textureMap[mtlName];
            vertSubMeshes.push_back(newSubmesh);
        }

        // ---------- FACE ----------
        else if (type == "f") {
            // ---------- COLLECT INDICES
            // Create an initial submesh for the current submesh. This ensures that if there is no
            // material (untextured model) than we can still create a submesh
            if(vertSubMeshes.size() == 0){
                subMesh newSubmesh;
                newSubmesh.textured = false;
                vertSubMeshes.push_back(newSubmesh);
            }

            // Vertices associated with a texture should follow "usemtl" call for that texture / material
            // so make the last submesh aded the current submesh for this face
            subMesh& currentMesh = vertSubMeshes.back();
            // Assume this mesh has no texture until a texture indice is found below
            currentMesh.textured = false;

            // Temporary storage for this polygons position and texture indices
            std::vector<int> vIdx;
            std::vector<int> tIdx;

            std::string vert;
            while (stream >> vert) {
                int v = 0, t = 0;
                // Find the slashes in the vertex string "v/t/n" or "v//n" or "v/t"
                size_t p1 = vert.find('/');         // first slash
                size_t p2 = vert.find('/', p1 + 1); // Second slash (if exists)

                // Extract position index (before first slash) and convert to 0-based
                v = std::stoi(vert.substr(0, p1)) - 1;

                // Extract texture index (between first and second slash) and convert to 0-based, if present 
                // If no slash, this vertex has no texture coordinates
                if (p1 != std::string::npos) {
                    t = std::stoi(vert.substr(p1 + 1, p2 - p1 - 1)) - 1; 
                    currentMesh.textured = true;
                }
                // Store indices for this face/polygon
                vIdx.push_back(v);
                tIdx.push_back(t);
            }
            // Need at least a triangle to generate face/polygon
            if (vIdx.size() < 3) continue;

            // ---------- FAN TRIANGULATION
            for (int i = 1; i < vIdx.size()-1; ++i) {
                // Start with the first triangle (should be {0,1,2} then {0,2,3} for triangle fan)
                int tri[3] = {0, i, i+1};
                // Triangle winding swapped if indicated in model constructor call to ensure normals face outward
                if (cwWinding) std::swap(tri[1], tri[2]);

                for (int k = 0; k < 3; ++k) {
                    // Create a new key containing the index of the vertex and 
                    vertexKey key{vIdx[tri[k]],tIdx[tri[k]]};
                    // "it" is an iterator (similiar to pointer) to the map element (not the value but the key-value pair itself)
                    auto it = vertexCache.find(key);

                    // If the key is not found, 'it' equals vertexCache.end()
                    // If found, it->first is the key, and it->second is the value (here, the vertex index)
                    // If vertex has already been created simply add it submesh indices
                    if (it != vertexCache.end()) currentMesh.indices.push_back(it->second);
                    else {
                        // Correct negative indices for OBJ (negative = relative to end of array)
                        int correctedKeyV = (key.v < 0) ? objPositions.size() + key.v + 1 : key.v;
                        int correctedKeyT = (key.t < 0) ? objTexcoords.size() + key.t + 1 : key.t;

                        uint32_t newIndex = static_cast<uint32_t>(vertices.size());

                        // vertex has not been created, so create it and add the position and texture data if it exists

                        // if (currentMesh.textured){
                        vertices.emplace_back(vertex{
                            objPositions[correctedKeyV],
                            currentMesh.textured ? objTexcoords[correctedKeyT] : glm::vec2(0.0f)
                        });
                        // Add new vertex to verices vector array and map the location to the 
                        // current submesh and the vertexCache hashtable for use on other faces if applicable
                        vertexCache[key] = newIndex;
                        currentMesh.indices.push_back(newIndex);
                    }
                }
            }
        }
    }


    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);  
    GLScopedVAO tempVAO(vao);
    GLScopedVBO tempVBO(vbo);

    // Need raw vertices from model because the VAO expects contigous memory
    glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(vertex),vertices.data(),GL_STATIC_DRAW);
    // positions at location 0
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex),(void*)offsetof(vertex, position));
    glEnableVertexAttribArray(0);
    // UVs at location 1
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(vertex),(void*)offsetof(vertex, uv));
    glEnableVertexAttribArray(1);


    for (const subMesh& mesh : vertSubMeshes) {
        gpuSubMesh gpuSub;

        gpuSub.textured = mesh.textured;
        gpuSub.indiceCount = mesh.indices.size();

        // Create EBO and Texture. These are unique to object submesh
        GLuint& ebo = gpuSub.ebo;
        GLuint& tex = gpuSub.tex;
        glGenBuffers(1, &gpuSub.ebo);
        glGenTextures(1, &gpuSub.tex);
        GLScopedEBO tempEBO(ebo);
        GLScopedTexture2D tempTex(tex);

        // Create EBO from the subMesh indices (these will still point to vertices in the object VAO)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t), mesh.indices.data(), GL_STATIC_DRAW);

        // Format for 4 channel color or 3 channel color (alpha or no alpha)
        GLenum format = (mesh.tex.channels == 4) ? GL_RGBA : GL_RGB;
        // Create texture on the GPU and load in the submesh texture data
        glTexImage2D(GL_TEXTURE_2D, 0, format, mesh.tex.w, mesh.tex.h, 0, format, GL_UNSIGNED_BYTE, mesh.tex.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        subMeshes.push_back(gpuSub);
    }

    glm::vec3 min(FLT_MAX);
    glm::vec3 max(-FLT_MAX);
    float rad = 0;

    for (const vertex& v : vertices)
    {
        min = glm::min(min, v.position);
        max = glm::max(max, v.position);
        rad = std::max(rad, glm::length(v.position));
    }

    glm::vec3 localCenter = (max + min) * 0.5f;
    glm::vec3 localHalfExtents = (max - min) * 0.5f;


    model mod(subMeshes,vertSubMeshes,vertices,vao,vbo);
    mod.localHalfExtents = localHalfExtents;
    mod.localCenter = localCenter;
    mod.radius = rad;
    return mod;

}




//---------------------- MODEL LOADING HELPERS ----------------------

std::string getDirectory(const std::string& filepath) {
    // find the location of the last "/" or "\\" this should be the directoy
    size_t pos = filepath.find_last_of("/\\");

    if (pos == std::string::npos) return "";
    return filepath.substr(0, pos + 1); // include trailing slash
}



texture loadTexture(const std::string& filepath) {
    // Directory converted from "dir\\file" to "dir/file"
    std::string correctedPath = filepath;
    std::replace(correctedPath.begin(), correctedPath.end(), '\\', '/');

    texture tex;
    stbi_set_flip_vertically_on_load(true);

    tex.data = stbi_load(correctedPath.c_str(), &tex.w, &tex.h, &tex.channels, 0);
    if (!tex.data) throw std::runtime_error("Failed to load texture: " + correctedPath);

    // Only 1, 3, or 4 channel images are supported in rasterizer
    if (!(tex.channels == 1 || tex.channels == 3 || tex.channels == 4)){
        stbi_image_free(tex.data);
        throw std::runtime_error("Unsupported image channel count: " + correctedPath);
    }
    return tex;
}



std::unordered_map<std::string, texture> loadMTL(const std::string& filepath) {
    std::unordered_map<std::string, texture> textureMap;
    std::ifstream file(filepath);
    if (!file) throw std::runtime_error("Failed to open MTL: " + filepath);

    // Texture locations will be relative to .mtl directory so we need to capture this
    std::string dir = getDirectory(filepath);
    texture* current = nullptr;
    std::string line;

    // itterate through the entire mtl file
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        // Load the name of the material into the map
        // This works because the "newmtl" will come before the "map_Kd" so current will be set before "map_Kd"
        if (type == "newmtl") {
            std::string name;
            ss >> name;
            textureMap[name] = {}; // Creates another element in the map if it does not exist
            current = &textureMap[name];
        } 
        // Load the texture into the material map
        else if (type == "map_Kd" && current) {
            std::string texPath;
            ss >> texPath;
            // Load texture from the path
            *current = loadTexture(dir + texPath);
        }
    }

    return textureMap;
}
