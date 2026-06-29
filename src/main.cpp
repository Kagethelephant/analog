// OpenGL
#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
// Project headers
#include "world/object.hpp"
#include "window/text.hpp"
#include "window/window.hpp"
#include "render/render.hpp"
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

   textRenderEngine text(programWindow, "../resources/font/novem___.ttf");


   //------------------- CREATE MODELS, OBJECTS AND LIGHTS ------------------------
   // Models load geometry once and can be used by many objects
   model arcanineModel("../resources/objects/Arcanine/Arcanine.obj");
   model cubeMod("../resources/objects/cube.obj");

   object arcanineObj(arcanineModel);
   arcanineObj.move(-10,0,-10);
   arcanineObj.scale(10,10,10);

   object cube(cubeMod);
   cube.move(0,0,-2);
   cube.scale(1,1,1);
   cube.color(Color::Yellow);

   // Light position and RGB color (0–1 range)
   light redLight(glm::vec3(15,5,5),glm::vec3(0.6,0.3,0.3));
   light blueLight(glm::vec3(-15,5,5),glm::vec3(0.3,0.3,0.6));

   //------------------- BIND OBJECTS AND LIGHTS TO RENDERERS ------------------------
   // Pass same camera, objects, and lights to both renderers to mirror screen output between the two
   gpuRenderEngine gpuRend(userCamera, programWindow);
   gpuRend.bindObject(arcanineObj);
   gpuRend.bindObject(cube);

   gpuRend.addLight(redLight);
   gpuRend.addLight(blueLight);


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
      const glm::vec2& resolution = programWindow.getFboSize();

      //------------------- RENDER PIPELINE ------------------------
      gpuRend.render();
      text.RenderText("GPU", resolution.x/2.0f, 10,Color::Green);
      text.RenderText("FPS: " + std::to_string(programWindow.getFPS()), 10, 10);

      // Renders the FBO to the screen, checks for events, updates FPS, etc.
      programWindow.frameUpdate();
      userCamera.updateView();
   }

   // Called after scope so all GLFW object destructors can be called
   return 0;
}

