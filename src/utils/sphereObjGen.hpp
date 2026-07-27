#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <cmath>
#include <filesystem>

namespace objGenerator
{

constexpr float PI = 3.14159265359f;


inline void createSphereOBJ(
    const std::string& filename,
    const std::string& mtlFilename,
    const std::string& textureFilename,
    float radius = 1.0f,
    int segments = 32,
    int rings = 16
)
{
    struct Vertex{
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };


    std::ofstream file(filename);

    if(!file.is_open()){
        std::cerr << "Failed to open OBJ file: " << filename << "\n";
        return;
    }


    // Write material reference
    file << "mtllib "
        << std::filesystem::path(mtlFilename).filename().string()
        << "\n\n";


    std::vector<Vertex> vertices;


    // Generate vertices
    for(int y = 0; y <= rings; y++)
    {
        float v = static_cast<float>(y) / rings;
        float phi = v * PI;

        for(int x = 0; x <= segments; x++)
        {
            float u = static_cast<float>(x) / segments;
            float theta = u * 2.0f * PI;


            glm::vec3 normal;

            normal.x = sin(phi) * cos(theta);
            normal.y = cos(phi);
            normal.z = sin(phi) * sin(theta);


            vertices.push_back({
                normal * radius,
                normal,
                {u, 1.0f - v}
            });
        }
    }


    // Write vertex positions
    for(const auto& vertex : vertices)
    {
        file << "v "
             << vertex.position.x << " "
             << vertex.position.y << " "
             << vertex.position.z
             << "\n";
    }


    // Write UVs
    for(const auto& vertex : vertices)
    {
        file << "vt "
             << vertex.uv.x << " "
             << vertex.uv.y
             << "\n";
    }


    // Write normals
    for(const auto& vertex : vertices)
    {
        file << "vn "
             << vertex.normal.x << " "
             << vertex.normal.y << " "
             << vertex.normal.z
             << "\n";
    }


    // Use material
    file << "\nusemtl SphereMaterial\n\n";


    // Write faces
    for(int y = 0; y < rings; y++)
    {
        for(int x = 0; x < segments; x++)
        {
            int i0 = y * (segments + 1) + x;
            int i1 = i0 + 1;
            int i2 = i0 + segments + 1;
            int i3 = i2 + 1;


            file << "f "
                << i0 + 1 << "/" << i0 + 1 << "/" << i0 + 1 << " "
                << i3 + 1 << "/" << i3 + 1 << "/" << i3 + 1 << " "
                << i2 + 1 << "/" << i2 + 1 << "/" << i2 + 1
                << "\n";


            file << "f "
                << i0 + 1 << "/" << i0 + 1 << "/" << i0 + 1 << " "
                << i1 + 1 << "/" << i1 + 1 << "/" << i1 + 1 << " "
                << i3 + 1 << "/" << i3 + 1 << "/" << i3 + 1
                << "\n";
        }
    }


    file.close();


    // Create MTL file
    std::ofstream mtl(mtlFilename);

    if(!mtl.is_open()){
        std::cerr << "Failed to open MTL file: " << mtlFilename << "\n";
        return;
    }


    mtl << "newmtl SphereMaterial\n\n";

    // Ambient color
    mtl << "Ka 1.0 1.0 1.0\n";

    // Diffuse color
    mtl << "Kd 1.0 1.0 1.0\n";

    // Specular color
    mtl << "Ks 0.0 0.0 0.0\n";

    // Diffuse texture
    mtl << "map_Kd "
        << std::filesystem::path(textureFilename).filename().string()
        << "\n";


    mtl.close();


    std::cout << "Created sphere OBJ: " << filename << "\n";
    std::cout << "Created sphere MTL: " << mtlFilename << "\n";
}

}
