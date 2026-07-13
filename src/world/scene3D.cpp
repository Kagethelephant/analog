#include "scene3D.hpp"

#include "utils/debug.hpp"
// Program headers
// OpenGL
// Std Libraries
#include <vector>
#include <iostream>



model3D scene3D::createModel(const std::string& filename, bool cwWinding){
   m_models.emplace_back(filename,cwWinding);
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



// void object3D::updateMassProperties()
// {
//    const object& m_object = m_scene->getObjects()[m_index];
//    const model& m_model = m_scene->getModels()[m_object.getModel()];
//    const rigidBody& base = m_model.getMassProperties();

//    glm::vec3 scale = m_object.getScale();

//    if(scale.x <= 0 || scale.y <= 0 || scale.z <= 0){
//       std::cout <<"Negative scale not supported for physics" << std::endl;
//    }
//    
//    m_body = m_model.generateMassProperties(scale);

//    std::cout << std::endl;
//    printMat3(m_body.inertia);
//    std::cout << "COM: (" << m_body.com.x << ", "<< m_body.com.y << ", "<< m_body.com.z << ")" << std::endl;
// }
