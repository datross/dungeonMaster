#include "camera.h"

Camera::Camera() 
{
}

void Camera::init(float fov, float aspectRatio){
    projection = glm::perspective<float>(glm::radians(fov), aspectRatio, 0., 100.);
}

glm::mat4 Camera::getVMatrix() {
  return glm::lookAt(position, 
                     position + direction,
                     glm::vec3(0 , 1, 0)); // TODO: CHECK THIS
}

glm::mat4 Camera::getPMatrix() {
    return projection;
}