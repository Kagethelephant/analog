#pragma once
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// Standard Libraries
#include <vector>
#include <unordered_map>


class window {

public:

   window(int height);
   ~window();


   std::unordered_map<int, int> prevKeyState;
   enum class KeyMode { Pressed, PressedOnce, Released };
   bool checkKey(int key, KeyMode mode = KeyMode::Pressed);

   void frameUpdate();

   // void resize();

   // void setFboHeight(int height) {
   //    aspectRatio = float(windowSize.x) / float(windowSize.y);
   //    fboSize.y = height;
   //    fboSize.x = int(fboSize.y * aspectRatio);
   //    // resize();
   // }

   GLuint getFbo() const { return fbo;}
   GLuint getColorTexture() const { return colorTex;}
   GLuint getVao() const { return UIvao;}
   float getAspectRatio() const { return aspectRatio;}
   const glm::vec2& getFboSize() const { return fboSize;}
   const glm::vec2& getWindowSize() const { return windowSize;}


   GLFWwindow* getHandle() {return win;}

   bool shouldClose() const {return glfwWindowShouldClose(win);}
   double getFrameElapsedTime() const {return frameTime;}
   int getFPS() const {return fps;}

   void close() {glfwSetWindowShouldClose(win, true);}

private:

   GLFWwindow* win;

   glm::vec2 fboSize;
   glm::vec2 windowSize;

   float aspectRatio;

   GLuint fbo = 0;
   GLuint colorTex = 0;
   GLuint depth = 0;

   GLuint UIvao;
   GLuint UIvbo;


   double lastTime = glfwGetTime();
   double frameTime;
   double currentTime;
   int frameCount = 0;
   int fps;

   /// @brief: Shader program to render 2D quads with textures
   GLuint shaderProgramUI;

   std::vector<float> quadVertices;

   bool resizePending = false;
};
