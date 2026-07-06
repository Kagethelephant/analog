#include "window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>


window::window(int height){

   //---------------------- INITIALIZE GLFW ----------------------
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   //---------------------- SET FBO AND WINDOW SIZE ----------------------
   // On start make the window the size of the display
   const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
   windowSize.x = mode->width;
   windowSize.y = mode->height;

   // FBO is the texture that we draw everything to. Lower resolution will give pixelated look
   aspectRatio = float(windowSize.x) / float(windowSize.y);


   //---------------------- SETUP GLFW ----------------------

   win = glfwCreateWindow(windowSize.x, windowSize.y, "The Game", NULL, NULL);
   glfwMakeContextCurrent(win);
   // Dont let the window height scale below FBO height
   // glfwSetWindowSizeLimits(win, GLFW_DONT_CARE, fboSize.y, GLFW_DONT_CARE, GLFW_DONT_CARE);
   // This disables vsync
   glfwSwapInterval(0); 
   // Initialize GLAD (Loads functions from the GPU)
   gladLoadGL();

   // Makes it so "this*" can be used to reference the GLFW window in this classes callback functions
   glfwSetWindowUserPointer(win, this); 
   glfwSetFramebufferSizeCallback(win, [](GLFWwindow* _win, int w, int h){

      auto* self = static_cast<window*>(glfwGetWindowUserPointer(_win));
      if (!self) return;

      self->windowSize.x  = w;
      self->windowSize.y = h;
      self->aspectRatio  = float(w) / float(h);
   });

}


bool window::checkKey(int key, KeyMode mode){
    int current = glfwGetKey(win, key);
    int previous = prevKeyState[key];

    bool result = false;
    switch(mode){
        case KeyMode::Pressed:
            result = (current == GLFW_PRESS);
            break;
        case KeyMode::PressedOnce:
            result = (current == GLFW_PRESS && previous != GLFW_PRESS);
            break;
        case KeyMode::Released:
            result = (current == GLFW_RELEASE && previous == GLFW_PRESS);
            break;
    }

    // update previous state for next frame
    prevKeyState[key] = current;
    return result;
}


void window::frameUpdate(){



   frameTime = glfwGetTime() - currentTime; // In seconds
   currentTime = glfwGetTime();
   frameCount++;
   // If a second has passed
   if (currentTime - lastTime >= 0.2f){
      // Calculate FPS and display it (e.g., in the window title or console)
      fps = (int)frameCount / (currentTime - lastTime);
      frameCount = 0;
      lastTime = currentTime;
   }

   glfwSwapBuffers(win);
   glfwPollEvents();
}


window::~window(){
   // Make sure the context is current before deleting GL objects
   if (win) glfwMakeContextCurrent(win);
   if (win) glfwDestroyWindow(win);

   glfwTerminate();
}
