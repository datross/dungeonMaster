#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <utility>
#include <string>
#include "glm.h"

enum EntityType {
	PLAYER,
	MONSTER,
	DROPABLE_ITEM,
	CHEST,
	DOOR
};

enum AnimsSet {
	ENCOUNTER = 0,
	END = 1,
	STAND = 2
};

class Animation {
public:
	float duration;
	std::pair< float,std::vector<glm::vec3> > anim;

	void loadAnimation(std::string path);
	void execute();
};

#endif
