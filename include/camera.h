#ifndef CAMERA_H
#define CAMERA_H

#include "glm.h"
#include <vector>

class Camera {
private:
  std::vector<float> position;
  std::vector<float> orientation;
  float focal;
public:
  glm::mat4 getMatrix();
};

#endif
