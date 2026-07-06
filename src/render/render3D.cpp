#include "render3D.hpp"
// Program headers
#include "RAIIWrapper.hpp"
#include "glm/fwd.hpp"
#include "utils/data.hpp"
#include "window/window.hpp"
#include "world/object.hpp"
#include "world/model.hpp"
#include <iostream>
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Std Libraries
#include <vector>



render3D::render3D(surface& surf) : m_renderSurface(surf){

   m_shaderProgram3D = createShaderProgram("../src/shaders/3d_vertex.glsl", "../src/shaders/3d_fragment.glsl");
};

model3D render3D::loadModel(const std::string& filename, bool cwWinding){
   m_models.emplace_back(filename,cwWinding);
   return model3D(m_models.size() - 1);
}

object3D render3D::createObject(model3D m){
   objects.emplace_back(m.index);
   std::cout << "Model Created" << std::endl;
   return object3D (this,objects.size() - 1);

}



void render3D::render(camera& cam){

   m_renderSurface.resize();
   glm::ivec2 s(m_renderSurface.getAspect() *800,  800);
   cam.setAspectRatio(m_renderSurface.getAspect());

   const glm::vec2& resolution = m_renderSurface.size();
   // Set OpenGL states that are agnostic of object or submesh 
   GLScopedFBO tempFBO(m_renderSurface.getFbo());                           // Window FBO to draw to
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

   // Tell GPU how many lights we have so it does not have to itterate to MAX_LIGHTS each fragment
   int lightCount = std::min(MAX_LIGHTS, (unsigned int)m_lightPositions.size() / 3);

   for (const object& obj : objects){

      const model& mod = m_models[obj.getModel()];

      const GLuint& vao = mod.getVao();
      const GLuint& vbo = mod.getVbo();

      glm::vec4 color = getColor(obj.color());

      // Setup the VBO using the VAO
      GLScopedVAO tempVAO(vao);
      GLScopedVBO tempVBO(vbo);



      // update the uniforms per fram to account for camera, object or light moves
      glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram3D, "view"),1,GL_FALSE,&cam.getViewMatrix()[0][0]);
      glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram3D, "project"),1,GL_FALSE,&cam.getProjectionMatrix()[0][0]);

      glUniform1i(glGetUniformLocation(m_shaderProgram3D, "lightCount"),lightCount);
      glUniform3fv(glGetUniformLocation(m_shaderProgram3D, "lightPos"),lightCount,&m_lightPositions[0]);
      glUniform3fv(glGetUniformLocation(m_shaderProgram3D, "lightCol"),lightCount,&m_lightColors[0]);

      glUniform3fv(glGetUniformLocation(m_shaderProgram3D, "objCol"),1,&color[0]);
      glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram3D, "model"),1,GL_FALSE,&obj.getModelMatrix()[0][0]);


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
   }
}

