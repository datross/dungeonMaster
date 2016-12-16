#ifndef ANIMATION_H
#define ANIMATION_H
#include <vector>
#include <utility>
#include <string>
#include "glm.h"

class Animation {
public:
	float duration;
	std::pair<float,std::vector<glm::vec3> > anim;

	void loadAnimation(std::string path);
	void execute();
};

#endif
