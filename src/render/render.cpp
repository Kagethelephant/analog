#include "render.hpp"
// Program headers
#include "RAIIWrapper.hpp"
#include "glm/fwd.hpp"
#include "utils/data.hpp"
#include "window/window.hpp"
#include "world/object.hpp"
#include "window/text.hpp"
#include "iostream"
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Std Libraries
#include <vector>



gpuRenderEngine::gpuRenderEngine( const window& win) : 
   // m_camera{cam},
   m_window{win}{ 
   // renderSurface(win,height,surface::resizeMode::fixedHeight){

   m_shaderProgram3D = createShaderProgram("../src/shaders/3d_vertex.glsl", "../src/shaders/3d_fragment.glsl");
   shaderProgramUI = createShaderProgram("../src/shaders/ui_vertex.glsl", "../src/shaders/ui_fragment.glsl");


   // DEFINE THE VERTEX DATA QUAD
   // -----------------------------------------------------------------------------------
   m_quad = {
      -1.0f,  1.0f,  0.0f,  1.0f, // x, y, u, v
      -1.0f, -1.0f,  0.0f,  0.0f,
       1.0f, -1.0f,  1.0f,  0.0f,

      -1.0f,  1.0f,  0.0f,  1.0f,
       1.0f, -1.0f,  1.0f,  0.0f,
       1.0f,  1.0f,  1.0f,  1.0f
   };

   // Create the VAO storing the vertice data for our full screen quad
   glGenVertexArrays(1, &m_quadVao);  
   glGenBuffers(1, &m_quadVbo);

   // Setup the VBO using the VAO
   GLScopedVAO tempVAO(m_quadVao);
   GLScopedVBO tempVBO(m_quadVbo);

   glBufferData(GL_ARRAY_BUFFER, m_quad.size() * sizeof(GLfloat), m_quad.data(), GL_STATIC_DRAW);
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
   // This tells GL to use the vertex attributes defined above (it does not do this by default)
   glEnableVertexAttribArray(0);  
   glEnableVertexAttribArray(1);  

   // renderSurface.size(s);
};



void gpuRenderEngine::draw(surface& surf){

   // Render the screen

   // Bind the main window framebuffer to draw to the screen
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   // Set the viewport, shader program and VAO with RAII wrappers that will reset to previous
   // OpenGL states at the end of the scope where they were bound
   GLScopedViewport winViewPort(0, 0, m_window.getWindowSize().x, m_window.getWindowSize().y);
   GLScopedProgram winProgram(shaderProgramUI);
   GLScopedVAO tempVAO(m_quadVao);
   // Select texture unit 0 and bind colorTex to GL_TEXTURE_2D on that unit
   glActiveTexture(GL_TEXTURE0);
   GLScopedTexture2D tempTexture(surf.getSurface());

   glUniform1i(glGetUniformLocation(shaderProgramUI, "screenTexture"), 0);
   glDrawArrays(GL_TRIANGLES, 0, 6);
}




// void gpuRenderEngine::render(){


//    renderSurface.resize();
//    glm::ivec2 s(m_window.getAspectRatio() *800,  800);
//    m_camera.setAspectRatio(m_window.getAspectRatio());

//    {

//       const glm::vec2& resolution = renderSurface.size();
//       // Set OpenGL states that are agnostic of object or submesh 
//       GLScopedFBO tempFBO(renderSurface.getFbo());                           // Window FBO to draw to
//       GLScopedViewport tempViewPort(0, 0, resolution.x, resolution.y);  // Viewport matching FBO size
//       GLScopedProgram tempProgram(m_shaderProgram3D);                   // 3D rendering shader program
//       GLScopedCapability tempCullEnable(GL_CULL_FACE,true);             // Backface culling enable
//       GLScopedCullFace tempCullMode(GL_BACK);                           // Ensure back face is culled rather than front
//       GLScopedCapability tempDepthEnable(GL_DEPTH_TEST, true);          // Depth buffer test
//       GLScopedActiveTexture tempActiveTex(GL_TEXTURE0);                 // Active texture (only texture 0 is used)

//       // Clear the FBO to remove what was rendered last frame
//       glm::vec4 bgColor = getColor(Color::Black);
//       glClearColor(bgColor[0],bgColor[1],bgColor[2],bgColor[3]);
//       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//       // Tell GPU how many lights we have so it does not have to itterate to MAX_LIGHTS each fragment
//       int lightCount = std::min(MAX_LIGHTS, (unsigned int)m_lightPositions.size() / 3);

//       for (const gpuObject& gpuObject : m_objects){

//          const object& objRef = gpuObject.obj;
//          const GLuint& vao = gpuObject.vao;
//          const GLuint& vbo = gpuObject.vbo;

//          glm::vec4 color = getColor(objRef.color());

//          // Setup the VBO using the VAO
//          GLScopedVAO tempVAO(vao);
//          GLScopedVBO tempVBO(vbo);


//          // update the uniforms per fram to account for camera, object or light moves
//          glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram3D, "view"),1,GL_FALSE,&m_camera.getViewMatrix()[0][0]);
//          glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram3D, "project"),1,GL_FALSE,&m_camera.getProjectionMatrix()[0][0]);

//          glUniform1i(glGetUniformLocation(m_shaderProgram3D, "lightCount"),lightCount);
//          glUniform3fv(glGetUniformLocation(m_shaderProgram3D, "lightPos"),lightCount,&m_lightPositions[0]);
//          glUniform3fv(glGetUniformLocation(m_shaderProgram3D, "lightCol"),lightCount,&m_lightColors[0]);

//          glUniform3fv(glGetUniformLocation(m_shaderProgram3D, "objCol"),1,&color[0]);
//          glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram3D, "model"),1,GL_FALSE,&objRef.getModelMatrix()[0][0]);


//          for (const gpuSubMesh& sub : gpuObject.subMeshes) {

//             const GLuint& ebo = sub.ebo;
//             const GLuint& tex = sub.tex;

//             GLScopedEBO tempEBO(ebo);
//             GLScopedTexture2D tempTexture(tex);

//             int textureIntBool;
//             textureIntBool = (sub.textured) ? 1 : 0;

//             glUniform1ui(glGetUniformLocation(m_shaderProgram3D, "hasTexture"), textureIntBool);
//             glUniform1i(glGetUniformLocation(m_shaderProgram3D, "diffuseTex"), 0);

//             glDrawElements(GL_TRIANGLES,sub.indiceCount, GL_UNSIGNED_INT, 0);
//          }
//       }
//    }
// }

