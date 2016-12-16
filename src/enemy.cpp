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

std::vector<glm::ivec2> Enemy::reach(glm::ivec2 target, Map* map) {
  std::vector<glm::ivec2> path;
  map->print();

  float totalDist = Utils::distance(position, target);
  std::cout << "Distance : " << totalDist << std::endl;

  //Find where he can move
  int i = 0;
  while (totalDist > 0) {
    std::cout << "Tour : " << i << std::endl;

    float newDist = Utils::distance(glm::ivec2(position.x-1, position.y), target);
    if (map->isCaseEmpty(position.x-1, position.y) && newDist < totalDist)
      std::cout << "Can move left. New dist : " << newDist << std::endl;

    newDist = Utils::distance(glm::ivec2(position.x+1, position.y), target);
    if (map->isCaseEmpty(position.x+1, position.y) && newDist < totalDist)
      std::cout << "Can move right. New dist : " << newDist << std::endl;

    newDist = Utils::distance(glm::ivec2(position.x, position.y-1), target);
    if (map->isCaseEmpty(position.x, position.y-1) && newDist < totalDist)
        std::cout << "Can move top. New dist : " << newDist << std::endl;

    newDist = Utils::distance(glm::ivec2(position.x, position.y+1), target);
    if (map->isCaseEmpty(position.x, position.y+1) && newDist < totalDist)
        std::cout << "Can move bot. New dist : " << newDist << std::endl;

    totalDist = newDist;
    i++;
  }

  return path;
}
