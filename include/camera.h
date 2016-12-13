#ifndef CAMERA_H
#define CAMERA_H

#include "glm.h"
#include <vector>

class Camera {
private:
  glm::vec3 position;
  glm::vec3 direction;
  float focal;
public:
  glm::mat4 getMatrix();
};

#endif
