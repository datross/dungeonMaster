#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <utility>
#include <string>
#include <SDL2/SDL.h>
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
	Animation();
	Animation(std::string path);
	~Animation();

	Uint32 beginning;

	float duration;
	std::vector<std::pair< float , std::vector< glm::vec3 > > > anim;

	void loadAnimation(std::string path);

	template <typename T>
	bool execute(T& entity);

	void print();
};

#endif
