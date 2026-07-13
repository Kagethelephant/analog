#pragma once
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// Standard Libraries
#include <unordered_map>

class frameBuffer {
public:
   virtual GLuint framebuffer() const = 0;
   virtual glm::ivec2 size() const = 0;
   virtual ~frameBuffer() = default;
};

class window : public frameBuffer {

public:

   window(int height);
   ~window();


   std::unordered_map<int, int> prevKeyState;
   enum class KeyMode { Pressed, PressedOnce, Released };
   bool checkKey(int key, KeyMode mode = KeyMode::Pressed);

   void present();


   float getAspectRatio() const { return m_aspectRatio;}
   const glm::vec2& getWindowSize() const { return m_size;}

   glm::ivec2 size() const override{return m_size;}


   bool shouldClose() const {return glfwWindowShouldClose(m_win);}
   double getFrameElapsedTime() const {return m_frameTime;}
   int getFPS() const {return m_fps;}

   void close() {glfwSetWindowShouldClose(m_win, true);}
   GLuint framebuffer() const override {return 0;}

private:

   GLFWwindow* m_win;

   glm::vec2 m_size;

   float m_aspectRatio;

   double m_lastTime = glfwGetTime();
   double m_frameTime;
   double m_currentTime;
   int m_frameCount = 0;
   int m_fps;
};
