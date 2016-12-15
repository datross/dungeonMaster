#ifndef CAMERA_H
#define CAMERA_H

#include "glm.h"
#include <vector>

class Camera {
public:
    glm::mat4 projection;
	
    Camera();
    void init(float fov, float aspectRatio);
    glm::mat4 getPMatrix();
    glm::mat4 getVMatrix();

    glm::vec3 position;
    glm::vec3 direction;
};

#endif
