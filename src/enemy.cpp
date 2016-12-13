#include "enemy.h"

Enemy::Enemy(glm::ivec2 position, glm::vec3 orientation,
            float scale, std::string id,
            unsigned int life, unsigned int defense, unsigned int power,
            unsigned int detectRange) :
                    Character(position, orientation, scale, id, life, defense, power),
                    detectRange(detectRange)
                    {}
Enemy::~Enemy(){}

bool Enemy::detect(Character* other){
  glm::ivec2 target = other->getPosition();
  if(abs(target[0] - position[0]) > 5)
    return false;
  else if ((abs(target[0] - position[0]) + abs(target[1] - position[1])) > 5)
    return false;
  else
    return true;
}
std::vector<glm::ivec2> Enemy::reach(glm::ivec2 target) {
  std::vector<glm::ivec2> path;
  return path;
}
