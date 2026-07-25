#pragma once
// Program headers
#include "object.hpp"
#include "model.hpp"
#include "camera.hpp"
// #include "shaders/shader.hpp"
// OpenGL
// #include <cstddef>
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// Standard Libraries
#include <vector>
#include <string>



struct model3D{
   model3D(std::size_t i) : index(i){};
   std::size_t index;
};


class light3D;
class object3D;
class camera3D;

/// @brief: GPU rendering engine to handle the OpenGL calls for 3D rendering for the provided
/// models / objects in 3D space. Draws to window FBO and window class handles rendering to GLFW window
class scene3D {

public:

   /// @brief: Create new GPU rendering engine
   /// @param cam: Camera to reference for view matrix and window size
   scene3D() : m_activeCamera(0) {m_cameras.emplace_back();};

   /// @brief: Load object data to GPU and memory to render in loop with "render"
   /// @param obj: Object to load
   model3D createModel(const std::string& filename, bool cwWinding = false);
   object3D createObject(model3D m);
   camera3D createCamera();
   light3D createLight();


   std::vector<model>& getModels(){return m_models;}
   std::vector<light>& getLights(){return m_lights;}
   std::vector<object>& getObjects(){return m_objects;}
   std::vector<camera>& getCameras(){return m_cameras;}
   
   camera& getActiveCamera() {return m_cameras[m_activeCamera];}
   void setActiveCamera(camera3D&  cam);


private:

   std::size_t m_activeCamera;

   std::vector<object> m_objects;
   std::vector<camera> m_cameras;
   std::vector<model> m_models;
   std::vector<light> m_lights;

};



class object3D{
public:
   object3D(scene3D* scene, std::size_t index) : m_scene(scene), m_index(index) {}
   object* operator->(){ return &m_scene->getObjects()[m_index];}
   // void updateMassProperties();
private:
   rigidBody m_body;
   scene3D* m_scene;
   std::size_t m_index;
};


class camera3D{
public:
   camera3D(scene3D* scene, std::size_t index) : m_scene(scene), m_index(index) {}
   camera* operator->(){ return &m_scene->getCameras()[m_index];}
   std::size_t getID() {return m_index;}

private:
   scene3D* m_scene;
   std::size_t m_index;
};

class light3D{
public:
   light3D(scene3D* scene, std::size_t index) : m_scene(scene), m_index(index) {}
   light* operator->(){ return &m_scene->getLights()[m_index];}

private:
   scene3D* m_scene;
   std::size_t m_index;
};


