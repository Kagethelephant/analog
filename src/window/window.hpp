#pragma once
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// Standard Libraries
#include <unordered_map>

class IResizeTarget
{
public:
    virtual glm::ivec2 size() const = 0;
    virtual ~IResizeTarget() = default;
};

class window : public IResizeTarget {

public:

   window(int height);
   ~window();


   std::unordered_map<int, int> prevKeyState;
   enum class KeyMode { Pressed, PressedOnce, Released };
   bool checkKey(int key, KeyMode mode = KeyMode::Pressed);

   void frameUpdate();


   float getAspectRatio() const { return aspectRatio;}
   const glm::vec2& getWindowSize() const { return windowSize;}

   glm::ivec2 size() const override{return windowSize;}


   bool shouldClose() const {return glfwWindowShouldClose(win);}
   double getFrameElapsedTime() const {return frameTime;}
   int getFPS() const {return fps;}

   void close() {glfwSetWindowShouldClose(win, true);}

private:

   GLFWwindow* win;

   glm::vec2 windowSize;

   float aspectRatio;

   double lastTime = glfwGetTime();
   double frameTime;
   double currentTime;
   int frameCount = 0;
   int fps;
};
