// OpenGL
#include <cstddef>
#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
// Project headers
#include "world/object.hpp"
#include "window/text.hpp"
#include "window/window.hpp"
#include "render/render.hpp"
#include "render/render3D.hpp"
// Standard Libraries
#include <math.h>
#include <string>



int main(int argc, char* argv[]){

   //------------------- INITIALIZE WINDOW RESOURCES ----------------------
   window programWindow(800);
   camera userCamera;
   // Base camera movement speeds (scaled by frame time in the main loop)
   double posSpeed = 10.0f; // Position units / sec
   double rotSpeed = 3.0f;  // Radians / sec

   surface renderSurface(programWindow,400,surface::resizeMode::fixedHeight);

   gpuRenderEngine gpuRend(programWindow);
   render3D rend(renderSurface);
   textRenderEngine textRend("../resources/font/novem___.ttf");


   //------------------- CREATE MODELS, OBJECTS AND LIGHTS ------------------------
   // Models load geometry once and can be used by many objects
   model3D arcanineModel = rend.loadModel("../resources/objects/Arcanine/Arcanine.obj");
   model3D cubeMod = rend.loadModel("../resources/objects/cube.obj");

   object3D arcanineObj = rend.createObject(arcanineModel);
   arcanineObj.move(-4,0,-10);
   arcanineObj.scale(10,10,10);


   object3D cube = rend.createObject(cubeMod);
   cube.move(2,0,-8);
   cube.color(Color::Yellow);

   object3D cube2 = rend.createObject(cubeMod);
   cube2.move(-1,0,-8);
   cube2.scale(.5,.5,.5);
   cube2.color(Color::Blue);

   // Light position and RGB color (0–1 range)
   light redLight(glm::vec3(15,5,5),glm::vec3(0.6,0.3,0.3));
   light blueLight(glm::vec3(-15,5,5),glm::vec3(0.3,0.3,0.6));

   //------------------- BIND OBJECTS AND LIGHTS TO RENDERERS ------------------------
   // Pass same camera, objects, and lights to both renderers to mirror screen output between the two
   rend.addLight(redLight);
   rend.addLight(blueLight);


   //------------------- PROGRAM LOOP ------------------------
   while(!programWindow.shouldClose()){

      //------------------- USER INPUT ------------------------
      // Multiply the speed of movement and rotation by the amount of time the last frame took
      // This will make it so very high and low frame rates will move relatively the same in real time
      double posDelta = posSpeed * programWindow.getFrameElapsedTime();
      double rotDelta = rotSpeed * programWindow.getFrameElapsedTime();

      if (programWindow.checkKey(GLFW_KEY_ESCAPE)) {programWindow.close();}
      if (programWindow.checkKey(GLFW_KEY_S)) {userCamera.move(0, 0, -posDelta);}
      if (programWindow.checkKey(GLFW_KEY_W)) {userCamera.move(0, 0, posDelta);}
      if (programWindow.checkKey(GLFW_KEY_A)) {userCamera.move(-posDelta, 0, 0);}
      if (programWindow.checkKey(GLFW_KEY_D)) {userCamera.move(posDelta, 0, 0);}
      if (programWindow.checkKey(GLFW_KEY_LEFT)) {userCamera.rotate(0, rotDelta, 0);}
      if (programWindow.checkKey(GLFW_KEY_RIGHT)) {userCamera.rotate(0, -rotDelta, 0);}
      if (programWindow.checkKey(GLFW_KEY_UP)) {userCamera.rotate(rotDelta, 0, 0);}
      if (programWindow.checkKey(GLFW_KEY_DOWN)) {userCamera.rotate(-rotDelta, 0, 0);}

      // Update the resolution per fram in case the window changes size
      const glm::vec2& resolution = renderSurface.size();

      //------------------- RENDER PIPELINE ------------------------
      rend.render(userCamera);

      textRend.RenderText(renderSurface,"GPU", resolution.x/2.0f, 10,Color::Green);
      textRend.RenderText(renderSurface,"FPS: " + std::to_string(programWindow.getFPS()), 10, 10);
      gpuRend.draw(rend.getSurface());

      // Renders the FBO to the screen, checks for events, updates FPS, etc.
      programWindow.frameUpdate();
      userCamera.updateView();
   }

   return 0;
}

