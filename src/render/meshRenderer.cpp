#include "meshRenderer.hpp"
// Program headers
#include "RAIIWrapper.hpp"
#include "glm/fwd.hpp"
#include "utils/data.hpp"
#include "window/window.hpp"
#include "world/object.hpp"
#include "world/model.hpp"
#include "world/scene3D.hpp"
#include <iostream>
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Std Libraries
#include <vector>



meshRenderer::meshRenderer() {
    m_shaderProgram3D = createShaderProgram("../src/shaders/3d_vertex.glsl", "../src/shaders/3d_fragment.glsl");
};


void meshRenderer::render(surface& m_renderSurface, scene3D& scene){

    camera& cam = scene.getActiveCamera();

    std::vector<model>& m_models = scene.getModels();
    std::vector<object>& objects = scene.getObjects();

    std::vector<light>& m_lights = scene.getLights();
    std::vector<gpuLight> lights;

    for (light l : m_lights){
        lights.emplace_back(l.t.position,l.getColor());
    }


    m_renderSurface.resize();
    glm::ivec2 s(m_renderSurface.getAspect() *800,  800);
    cam.setAspectRatio(m_renderSurface.getAspect());


    const glm::vec2& resolution = m_renderSurface.size();
    // Set OpenGL states that are agnostic of object or submesh 
    GLScopedFBO tempFBO(m_renderSurface.getFbo());                    // Window FBO to draw to
    GLScopedViewport tempViewPort(0, 0, resolution.x, resolution.y);  // Viewport matching FBO size
    GLScopedProgram tempProgram(m_shaderProgram3D);                   // 3D rendering shader program
    GLScopedCapability tempCullEnable(GL_CULL_FACE,true);             // Backface culling enable
    GLScopedCullFace tempCullMode(GL_BACK);                           // Ensure back face is culled rather than front
    GLScopedCapability tempDepthEnable(GL_DEPTH_TEST, true);          // Depth buffer test
    GLScopedActiveTexture tempActiveTex(GL_TEXTURE0);                 // Active texture (only texture 0 is used)

    // Clear the FBO to remove what was rendered last frame
    glm::vec4 bgColor = getColor(Color::Black);
    glClearColor(bgColor[0],bgColor[1],bgColor[2],bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // DOES NOT HAVE RAII WRAPPER ! ! ! ! ! ! ! ! !
    GLuint lightSSBO;
    glGenBuffers(1, &lightSSBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER,lights.size() * sizeof(gpuLight),lights.data(),GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,0,lightSSBO);


    // DOES NOT HAVE RAII WRAPPER ! ! ! ! ! ! ! ! !

    // Tell GPU how many lights we have so it does not have to itterate to MAX_LIGHTS each fragment
    // int lightCount = std::min(MAX_LIGHTS, (unsigned int)m_lightPositions.size() / 3);

    for (const object& obj : objects){

        const model& mod = obj.getModel();

        const GLuint& vao = mod.getVao();
        const GLuint& vbo = mod.getVbo();

        glm::vec4 color = getColor(obj.color());

        // Setup the VBO using the VAO
        GLScopedVAO tempVAO(vao);
        GLScopedVBO tempVBO(vbo);


        int shadingMode = 0;

        // update the uniforms per fram to account for camera, object or light moves
        glUniform1i(glGetUniformLocation(m_shaderProgram3D, "shadingMode"),shadingMode);
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram3D, "view"),1,GL_FALSE,&cam.getViewMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram3D, "project"),1,GL_FALSE,&cam.getProjectionMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram3D, "model"),1,GL_FALSE,&obj.getModelMatrix()[0][0]);

        glUniform3fv(glGetUniformLocation(m_shaderProgram3D, "objCol"),1,&color[0]);


        for (const gpuSubMesh& sub : mod.getSubMeshes()) {

            const GLuint& ebo = sub.ebo;
            const GLuint& tex = sub.tex;

            GLScopedEBO tempEBO(ebo);
            GLScopedTexture2D tempTexture(tex);

            int textureIntBool;
            textureIntBool = (sub.textured) ? 1 : 0;

            glUniform1ui(glGetUniformLocation(m_shaderProgram3D, "hasTexture"), textureIntBool);
            glUniform1i(glGetUniformLocation(m_shaderProgram3D, "diffuseTex"), 0);

            glDrawElements(GL_TRIANGLES,sub.indiceCount, GL_UNSIGNED_INT, 0);
        }

        GLScopedVAO obbVAO(obj.vao);
        GLScopedEBO obbEBO(obj.ebo);

        glm::vec4 wireColor = getColor(Color::Yellow);
        glm::mat4 identity(1.0f);

        shadingMode = 1;
        glUniform1ui(glGetUniformLocation(m_shaderProgram3D, "hasTexture"), 0);
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram3D, "model"),1,GL_FALSE,&identity[0][0]);
        glUniform1i(glGetUniformLocation(m_shaderProgram3D, "shadingMode"),shadingMode);
        glUniform1i(glGetUniformLocation(m_shaderProgram3D, "enableShading"), false);
        glUniform3fv(glGetUniformLocation(m_shaderProgram3D, "objCol"),1,&wireColor[0]);
        glPolygonOffset(-1.0f, -1.0f);
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        // GLScopedCapability depth(GL_DEPTH_TEST, false);

        glDrawElements(GL_LINES,24,GL_UNSIGNED_INT,0);

        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glDisable(GL_POLYGON_OFFSET_FILL);
    }
}


