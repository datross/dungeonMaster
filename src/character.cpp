#include "character.h"

#include <iostream>

using namespace std;

Character::Character(){}
Character::Character(glm::ivec2 position, glm::vec3 orientation,
                    float scale, string id,
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

void Character::setPosition(glm::ivec2 pos) {
        position = pos;
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

void Character::print(){
  cout << "******Character******" << '\n';
  cout << "Id : " << id << endl;
  cout << "position : (" << position[0] << ";" << position[1] << ")" <<'\n';
  cout << "life/defense/power : (" << life << ";" << defense << ";" << power <<")" <<'\n';
  cout << "******Character - end******" << '\n';
}
