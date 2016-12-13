#include "camera.h"

glm::mat4 Camera::getMatrix(){
  return glm::lookAt(position, 
                     position + direction,
                     glm::vec3(0 , 0, 1)); // TODO: CHECK THIS
}
 