#include "enemy.h"
#include <iostream>
#include "utils.h"

#include "map.h"

Enemy::Enemy(glm::ivec2 position, glm::vec3 orientation,
            float scale, std::string id,
            unsigned int life, unsigned int defense, unsigned int power,
            unsigned int detectRange,
			std::shared_ptr<Mesh> mesh_ptr, std::shared_ptr<glimac::Program> shaders_ptr) :
	            Character(position, orientation, scale, id, life, defense, power, mesh_ptr, shaders_ptr),
	            detectRange(detectRange)
	            {}
Enemy::~Enemy(){}

bool Enemy::detect(Character* other){
  glm::ivec2 target = other->position;
  if(abs(target[0] - position[0]) > 5)
    return false;
  else if ((abs(target[0] - position[0]) + abs(target[1] - position[1])) > 5)
    return false;
  else
    return true;
}

Movement Enemy::reach(int targetIndex, Map* map) {
    std::vector<std::vector<unsigned int>> distances = map->getDistance(targetIndex);

    if (distances[position.y][position.x] == 255) {
        std::cout << "The enemy couldn't reach the player." << std::endl;
        return MOVEMENT_NONE;
    }

    if (position.x-1 >= 0) {
        if (distances[position.y][position.x-1] == distances[position.y][position.x]-1) {
			//MOVEMENT_LEFT;
			orientation.x = -1;
			orientation.z = 0;
        }
    }
    if (position.x+1 < (int) map->datas[0].size()) {
        if (distances[position.y][position.x+1] == distances[position.y][position.x]-1) {
			//MOVEMENT_RIGHT;
			orientation.x = 1;
			orientation.z = 0;
        }
    }
    if (position.y-1 >= 0) {
        if (distances[position.y-1][position.x] == distances[position.y][position.x]-1) {
			//MOVEMENT_FORWARD;
			orientation.x = 0;
			orientation.z = -1;
        }
    }
    if (position.y+1 < (int) map->datas.size()) {
        if (distances[position.y+1][position.x] == distances[position.y][position.x]-1) {
			//MOVEMENT_BACKWARD;
			orientation.x = 0;
			orientation.z = 1;
        }
    }

	return MOVEMENT_FORWARD;
}
