#include "enemy.h"
#include <iostream>
#include "utils.h"

#include "map.h"

Enemy::Enemy(glm::ivec2 position, glm::vec3 orientation,
            float scale, std::string id,
            unsigned int life, unsigned int defense, unsigned int power,
            unsigned int detectRange) :
                    Character(position, orientation, scale, id, life, defense, power),
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

Movement Enemy::reach(glm::ivec2 target, Map* map) {
  
  std::vector<std::vector<unsigned int>> distances = map->getDistance(0); //TODO Modifier le 0 avec le nombre du bon joueur
  
  
  if (distances[position.y][position.x] == 255) {
        std::cout << "The enemy couldn't reach the player." << std::endl;
        return MOVEMENT_NONE;
  }
  
  // TODO Vérification sortie du mur
  if (position.x-1 >= 0) {
    if (distances[position.y][position.x-1] == distances[position.y][position.x]-1)
        return MOVEMENT_LEFT;
  }
  if (position.x+1 < map->datas[0].size()) {
    if (distances[position.y][position.x+1] == distances[position.y][position.x]-1)
        return MOVEMENT_RIGHT;
  }
  if (position.y-1 >= 0) {
    if (distances[position.y-1][position.x] == distances[position.y][position.x]-1)
        return MOVEMENT_FORWARD;
  }
  if (position.y+1 < map->datas.size()) {
    if (distances[position.y+1][position.x] == distances[position.y][position.x]-1)
        return MOVEMENT_BACKWARD;
  }
}