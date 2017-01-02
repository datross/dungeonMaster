#include "camera.h"

Camera::Camera() 
{
}

void Camera::init(float fov, float aspectRatio){
    projection = glm::perspective<float>(glm::radians(fov), aspectRatio, 0.01, 100.);
}

glm::mat4 Camera::getVMatrix() {
    direction.x = glm::cos(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x));
    direction.z = glm::cos(glm::radians(rotation.y)) * glm::sin(glm::radians(rotation.x));
    direction.y = glm::sin(glm::radians(rotation.y));
    
    glm::vec3 local_left = glm::vec3(glm::sin(glm::radians(rotation.x)), 0, -glm::cos(glm::radians(rotation.x)));
    
    return glm::lookAt(position, position + direction, glm::cross(direction, local_left)); // TODO: CHECK THIS

}

glm::mat4 Camera::getPMatrix() {
    return projection;
}