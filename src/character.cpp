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

glm::ivec2 computeSteppedDirection(glm::vec3 orientation) {
    glm::ivec2 direction;
    if(orientation.x > 0 && orientation.z > 0) {
        if(orientation.x > orientation.z)
            direction = glm::ivec2(1, 0);
        else
            direction = glm::ivec2(0, 1);
    } else if(orientation.x < 0 && orientation.z > 0) {
        if(-orientation.x > orientation.z)
            direction = glm::ivec2(-1, 0);
        else
            direction = glm::ivec2(0, 1);
    } else if(orientation.x < 0 && orientation.z < 0) {
        if(orientation.x < orientation.z)
            direction = glm::ivec2(-1, 0);
        else
            direction = glm::ivec2(0, -1);
    } else if(orientation.x > 0 && orientation.z < 0) {
        if(orientation.x > -orientation.z)
            direction = glm::ivec2(1, 0);
        else
            direction = glm::ivec2(0, -1);
    }
    return direction;
}

glm::vec3 Character::getVisualPosition(Uint32 time) {
    glm::ivec2 direction_i = computeSteppedDirection(orientation);
    glm::vec3 direction = glm::vec3(direction_i.x, 0, direction_i.y);
    return glm::vec3(position.x, 0, position.y) + direction * anim_time_normalized;
}

void Character::move (Movement movement, Uint32 time){
    position += computeSteppedDirection(orientation);
    anim_start_time = time;
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
