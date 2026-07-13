// OpenGL
#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
// Project headers
#include "window/window.hpp"
#include "render/renderSystem.hpp"
#include "world/scene3D.hpp"
#include "utils/debug.hpp"
// Standard Libraries
#include <math.h>
#include <string>
#include <iostream>



int main(int argc, char* argv[]){

   //------------------- INITIALIZE WINDOW RESOURCES ----------------------
   window programWindow(800);

   surface renderSurface(programWindow,800,surface::resizeMode::fixedHeight);

   renderSystem gpuRend(programWindow);


   scene3D scene;
   camera3D userCamera = scene.createCamera();
   camera3D bottomCamera = scene.createCamera();
   //------------------- CREATE MODELS, OBJECTS AND LIGHTS ------------------------
   // Models load geometry once and can be used by many objects
   model3D arcanineModel = scene.createModel("../resources/objects/Arcanine/Arcanine.obj");
   model3D cubeMod = scene.createModel("../resources/objects/cube.obj");
   model3D tetMod = scene.createModel("../resources/objects/tetrahedron.obj");
   model3D cylinderMod = scene.createModel("../resources/objects/cylinder.obj");

   // object3D arcanineObj = scene.createObject(arcanineModel);
   // arcanineObj->move(-4,0,-10);
   // arcanineObj->scale(10,10,10);
   // arcanineObj.updateMassProperties();
   


   object3D cube = scene.createObject(cubeMod);
   cube->move(2,0,-8);
   cube->color(Color::Yellow);

   object3D cube2 = scene.createObject(cubeMod);
   cube2->move(-6,0,-4);
   // cube2->scale(.5,1,.5);
   cube2->color(Color::Blue);

   object3D cube3 = scene.createObject(cubeMod);
   cube3->move(-4,0,-8);
   cube3->scale(4,1,1);
   cube3->color(Color::Purple);

   object3D tet = scene.createObject(tetMod);
   tet->move(3,0,-4);
   tet->color(Color::Red);

   object3D sphere = scene.createObject(cylinderMod);
   sphere->move(7,0,-4);
   sphere->color(Color::Green);


   glm::vec3 force(0.0f,0.0f,-10.0f);
   // glm::vec3 point(0,2,0);
   glm::vec3 r(-1,1,0);

   // glm::vec3 r = point - glm::vec3(-4,0,-8);
   glm::vec3 t = glm::cross(r,force);
   glm::vec3 a = glm::inverse(cube->getBody().inertia) * t;
   float accTime = 0.1f;
   glm::vec3 aSpd = a * accTime;

   glm::vec3 acc = force/cube->getBody().mass;
   glm::vec3 tSpd = acc * accTime * 5.0f;

   




   //------------------- BIND OBJECTS AND LIGHTS TO RENDERERS ------------------------
   // Pass same camera, objects, and lights to both renderers to mirror screen output between the two
   light3D light1 = scene.createLight();
   light1->setColor(0.3f,0.3f,0.6f);
   light1->setPosition(15,5,5);

   light3D light2 = scene.createLight();
   light2->setColor(0.6f,0.3f,0.3f);
   light2->setPosition(-15,5,5);

   // light3D light2 = scene.createLight();
   // scene.createLight(glm::vec3(15,5,5),glm::vec3(0.6,0.3,0.3));
   // scene.createLight(glm::vec3(-15,5,5),glm::vec3(0.3,0.3,0.6));


   // Base camera movement speeds (scaled by frame time in the main loop)
   double posSpeed = 10.0f; // Position units / sec
   double rotSpeed = 3.0f;  // Radians / sec
   //------------------- PROGRAM LOOP ------------------------
   while(!programWindow.shouldClose()){

      //------------------- USER INPUT ------------------------
      // Multiply the speed of movement and rotation by the amount of time the last frame took
      // This will make it so very high and low frame rates will move relatively the same in real time
      double posDelta = posSpeed * programWindow.getFrameElapsedTime();
      double rotDelta = rotSpeed * programWindow.getFrameElapsedTime();

      glm::vec3 spin = aSpd * (float)programWindow.getFrameElapsedTime();
      glm::vec3 move = tSpd * (float)programWindow.getFrameElapsedTime();
      cube->rotate(spin.x,spin.y,spin.z);
      cube->move(move.x, move.y, move.z);


      if (programWindow.checkKey(GLFW_KEY_ESCAPE)) {programWindow.close();}
      if (programWindow.checkKey(GLFW_KEY_S)) {userCamera->move(0, 0, -posDelta);bottomCamera->move(0, 0, -posDelta);}
      if (programWindow.checkKey(GLFW_KEY_W)) {userCamera->move(0, 0, posDelta);bottomCamera->move(0, 0, posDelta);}
      if (programWindow.checkKey(GLFW_KEY_A)) {userCamera->move(-posDelta, 0, 0);bottomCamera->move(-posDelta, 0, 0);}
      if (programWindow.checkKey(GLFW_KEY_D)) {userCamera->move(posDelta, 0, 0); bottomCamera->move(posDelta, 0, 0);}
      if (programWindow.checkKey(GLFW_KEY_LEFT)) {userCamera->rotate(0, rotDelta, 0);}
      if (programWindow.checkKey(GLFW_KEY_RIGHT)) {userCamera->rotate(0, -rotDelta, 0);}
      if (programWindow.checkKey(GLFW_KEY_UP)) {userCamera->rotate(rotDelta, 0, 0);}
      if (programWindow.checkKey(GLFW_KEY_DOWN)) {userCamera->rotate(-rotDelta, 0, 0);}

      // Update the resolution per fram in case the window changes size
      const glm::vec2& resolution = renderSurface.size();

      //------------------- RENDER PIPELINE ------------------------
      scene.setActiveCamera(userCamera);
      gpuRend.drawScene(renderSurface,scene);

      gpuRend.RenderText(renderSurface,"GPU", resolution.x/2.0f, 10,Color::Green);
      gpuRend.RenderText(renderSurface,"FPS: " + std::to_string(programWindow.getFPS()), 10, 10);

      gpuRend.blit(renderSurface,programWindow);

      // Renders the FBO to the screen, checks for events, updates FPS, etc.
      programWindow.present();
      // userCamera.updateView();
   }

   return 0;
}

