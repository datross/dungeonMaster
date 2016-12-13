#include "character.h"
Character::Character(){}
Character::Character(glm::ivec2 position, glm::vec3 orientation,
                    float scale, std::string id,
                    unsigned int life, unsigned int defense, unsigned int power) :
                    position(position),
                    orientation(orientation),
                    scale(scale),
                    id(id),
                    life(life),
                    defense(defense),
                    power(power){}
Character::~Character(){}


glm::ivec2 Character  ::getPosition() const {
  return position;
}

void Character::move (Movement movement, Uint32 time){
  return;
}
void Character::orient (Direction direction, Uint32 time){
  return;
}
void Character::attack (Character* enemy){
  enemy->defend(power);
}
void Character::defend (unsigned int amountAttack){
  life -= (amountAttack - defense);
  if(!life) death();
}
void Character::death() {
  return;
}
