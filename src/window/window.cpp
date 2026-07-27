#include "window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>


window::window(int height){

    //---------------------- INITIALIZE GLFW ----------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    //---------------------- SET FBO AND WINDOW SIZE ----------------------
    // On start make the window the size of the display
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    m_size.x = mode->width;
    m_size.y = mode->height;

    // FBO is the texture that we draw everything to. Lower resolution will give pixelated look
    m_aspectRatio = float(m_size.x) / float(m_size.y);


    //---------------------- SETUP GLFW ----------------------

    m_win = glfwCreateWindow(m_size.x, m_size.y, "The Game", NULL, NULL);
    glfwMakeContextCurrent(m_win);
    // Dont let the window height scale below FBO height
    // glfwSetWindowSizeLimits(win, GLFW_DONT_CARE, fboSize.y, GLFW_DONT_CARE, GLFW_DONT_CARE);
    // This disables vsync
    glfwSwapInterval(0); 
    // Initialize GLAD (Loads functions from the GPU)
    gladLoadGL();


    std::cout << "OpenGL Vendor:  "<< glGetString(GL_VENDOR)<< "\n";
    std::cout << "GPU Renderer:   "<< glGetString(GL_RENDERER)<< "\n";
    std::cout << "OpenGL Version: "<< glGetString(GL_VERSION)<< "\n";
    std::cout << "GLSL Version:   "<< glGetString(GL_SHADING_LANGUAGE_VERSION)<< "\n";

    // Makes it so "this*" can be used to reference the GLFW window in this classes callback functions
    glfwSetWindowUserPointer(m_win, this); 
    glfwSetFramebufferSizeCallback(m_win, [](GLFWwindow* _win, int w, int h){

        auto* self = static_cast<window*>(glfwGetWindowUserPointer(_win));
        if (!self) return;

        self->m_size.x  = w;
        self->m_size.y = h;
        self->m_aspectRatio  = float(w) / float(h);
    });

}


bool window::checkKey(int key, KeyMode mode){
    int current = glfwGetKey(m_win, key);
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


void window::present(){



    m_frameTime = glfwGetTime() - m_currentTime; // In seconds
    m_currentTime = glfwGetTime();
    m_frameCount++;
    // If a second has passed
    if (m_currentTime - m_lastTime >= 0.2f){
        // Calculate FPS and display it (e.g., in the window title or console)
        m_fps = (int)m_frameCount / (m_currentTime - m_lastTime);
        m_frameCount = 0;
        m_lastTime = m_currentTime;
    }

    glfwSwapBuffers(m_win);
    glfwPollEvents();
}


window::~window(){
    // Make sure the context is current before deleting GL objects
    if (m_win) glfwMakeContextCurrent(m_win);
    if (m_win) glfwDestroyWindow(m_win);

    glfwTerminate();
}
