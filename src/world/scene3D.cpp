#include "scene3D.hpp"

#include "utils/debug.hpp"
#include "objLoader.hpp"
// Program headers
// OpenGL
// Std Libraries
#include <vector>
#include <iostream>



model3D scene3D::createModel(const std::string& filename, bool cwWinding){
   model m = loadModel(filename,cwWinding);
   m_models.push_back(m);
   return model3D(m_models.size() - 1);
}

object3D scene3D::createObject(model3D m){
   m_objects.emplace_back(this->m_models[m.index]);
   m_objects[m_objects.size() - 1].scale(1.0f,1.0f,1.0f);
   return object3D (this,m_objects.size() - 1);
}

camera3D scene3D::createCamera(){
   m_cameras.emplace_back();
   return camera3D (this,m_cameras.size() - 1);
}

light3D scene3D::createLight(){
   m_lights.emplace_back();
   return light3D (this,m_lights.size() - 1);
}


void scene3D::setActiveCamera(camera3D&  cam) {m_activeCamera = cam.getID();}

