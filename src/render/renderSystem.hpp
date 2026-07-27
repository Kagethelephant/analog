#pragma once
// Program headers
#include "render/meshRenderer.hpp"
#include "render/textRenderer.hpp"
#include "window/window.hpp"
#include "shaders/shader.hpp"
#include "surface.hpp"
#include "window/window.hpp"
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Standard Libraries
#include <vector>


/// @brief: GPU rendering engine to handle the OpenGL calls for 3D rendering for the provided
/// models / objects in 3D space. Draws to window FBO and window class handles rendering to GLFW window
class renderSystem {

public:

    /// @brief: Create new GPU rendering engine
    /// @param cam: Camera to reference for view matrix and window size
    renderSystem(const window& win);

    void blit(surface& surf, const frameBuffer& target);
    void blit(surface& surf, const frameBuffer& target, float x, float y);
    void blit(surface& surf, const frameBuffer& target, float x, float y, float w, float h);

    void drawScene(surface& surf, scene3D& scene){m_sceneRenderer.render(surf, scene);}


    void RenderText(surface& surf,std::string text, float x, float y, Color col = Color::White){

        m_textRender.RenderText(surf,text,x,y, col);
    }

private:

    void render(surface& surf);

    /// @brief: Shader program to render 2D quads with textures
    GLuint m_shaderProgramUI;

    const window& m_window;

    meshRenderer m_sceneRenderer;
    textRenderEngine m_textRender = textRenderEngine("../resources/font/novem___.ttf");

    GLuint m_quadVao;
    GLuint m_quadVbo;

    std::vector<float> m_quad;
};

