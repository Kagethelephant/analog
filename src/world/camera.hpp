
#pragma once
// Project Libraries
#include "glm/ext/matrix_clip_space.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
// #include "world/object.hpp"
#include "transform.hpp"



//---------------------- CAMERA ----------------------

/// @brief: Represents the viewer in the rendering pipeline and defines how the 3D scene
/// is observed and projected onto the screen. Stores the camera's position, orientation,
/// and the matrices required to transform world-space geometry into screen-space.
class camera {

public:

   /// @brief: Create camera associated with window
   /// @param win: window the camera renders to (used to obtain viewport aspect ratio)
   /// @param fov: vertical field of view of the camera in degrees
   camera(float ar = 1, float fov = 70) : m_aspectRatio(ar), m_fov(fov){
      m_projectionMatrix = glm::perspective(glm::radians(m_fov),m_aspectRatio,m_near,m_far);
      m_viewMatrix = glm::lookAt( m_transform.position, m_transform.position + m_transform.forward(), m_transform.up());
   }

   /// @brief: Moves camera according to the direction the camera is facing. ie. z moves forward
   /// and backward relative to camera point direction. x moves right and left (relative)
   /// @param x: Move sideways (right vector of camera)
   /// @param y: Move in the up direction ( up vector of camera)
   /// @param z: Move in the direction of forward vector of camera
   void move(float x, float y, float z);

   /// @brief: Rotates along right, forward and up direction vectors of camera (relative)
   /// @param u: Rotate around the right vector of the camera
   /// @param v: Rotate around the up vector of the camera
   /// @param w: Rotate around the forward vector of the camera
   void rotate(float pitch, float yaw, float roll);



   /// @brief: Get camera view matrix
   const glm::mat4& getViewMatrix() const {return (m_viewMatrix);};
   /// @brief: Get camera projection matrix
   const glm::mat4& getProjectionMatrix() const {return (m_projectionMatrix);};


   /// @brief: Set far plane position of camera, and update projection matrix
   /// @param far: far plane location as float 
   void setFarPlane(float far) {
      m_far = far;
      m_projectionMatrix = glm::perspective(glm::radians(m_fov),m_aspectRatio,m_near,m_far);
   };

   /// @brief: Set near plane position of camera, and update projection matrix
   /// @param near: near plane location as float 
   void setNearPlane(float near) {
      m_near = near;
      m_projectionMatrix = glm::perspective(glm::radians(m_fov),m_aspectRatio,m_near,m_far); 
   };

   /// @brief: Set field of view of camera, and update projection matrix
   /// @param fov: field of view of the camera
   void setFOV(float fov) {
      m_fov = fov; 
      m_projectionMatrix = glm::perspective(glm::radians(m_fov),m_aspectRatio,m_near,m_far);
   };

   /// @brief: Set aspect ratio (only updates projection matrix if changed)
   /// @param fov: field of view of the camera
   void setAspectRatio(float ar) {
      if(m_aspectRatio != ar){ 
         m_aspectRatio = ar; 
         m_projectionMatrix = glm::perspective(glm::radians(m_fov),m_aspectRatio,m_near,m_far); 
      }
   };

private:

   /// @brief: Field of view of view frustum
   float m_fov;
   /// @brief: Far plane of view frustum
   float m_far = 1000.0f;
   /// @brief: Near plane of view frustum
   float m_near = 0.1f;

   float m_aspectRatio;

   /// @brief: View matrix used for both GPU and CPU rendering.
   /// Transforms vertices from world space into camera (view) space, where the camera is treated as if it were at the origin
   /// looking down the negative z-axis. Conceptually, this moves the entire scene relative to the camera rather than moving
   /// the camera itself. Essential for positioning and orienting objects correctly from the camera's point of view.
   glm::mat4 m_viewMatrix;

   /// @brief: Projection matrix used for both GPU and CPU rendering. Transforms vertices from camera (view) space into 
   /// clip space, defining how the 3D scene is projected onto a 2D screen. Encodes the field of view, aspect ratio, 
   /// and near/far clipping planes, producing the foreshortening effect that makes distant objects appear smaller.
   /// Conceptually, it determines the volume of space that will be visible on the screen and maps that volume into the canonical
   /// cube used by the rasterizer (-1 to 1 in x, y, z in normalized device coordinates). Essential for accurately projecting
   /// 3D geometry onto a 2D viewport and ensuring consistent rendering between CPU and GPU pipelines.
   glm::mat4 m_projectionMatrix;

   transform m_transform;
};
