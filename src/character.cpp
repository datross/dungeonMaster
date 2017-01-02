
#include "character.h"

#include <iostream>

using namespace std;

Character::Character(){}
Character::Character(glm::ivec2 position, glm::vec3 orientation,
                    float scale, string id,
                    unsigned int life, unsigned int defense, unsigned int power,
					shared_ptr<Mesh> mesh_ptr, shared_ptr<glimac::Program> shaders_ptr) :
	                    position(position), orientation(orientation), anim_start_time(0), speed(4.),
	                    scale(scale), id(id),
	                    life(life), defense(defense), power(power),
						mesh_ptr(mesh_ptr),	shaders_ptr(shaders_ptr)
						{}
Character::~Character(){}

void Character::setDatas(std::vector<std::vector<int> >* datas) {
	this->mapDatas = datas;
}

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

bool Character::movementAnimationFinished(Uint32 time) {
    return (Uint32)(1000./speed) + anim_start_time < time;
}

// TODO altitude de la caméra ?
glm::vec3 Character::getVisualPosition(Uint32 time) {
    if(movementAnimationFinished(time)) { // l'anim de déplacement est terminée
        return glm::vec3(position.x, 0.6, position.y);
    }

//     glm::ivec2 direction_i = computeSteppedDirection(orientation);
    glm::vec3 direction = glm::vec3(position.x - position_prec.x, 0, position.y - position_prec.y);
    return glm::vec3(position_prec.x, 0.6, position_prec.y) + direction * (float)(0.001 * speed *(time - anim_start_time));
}

bool Character::canMove(glm::ivec2 newPos) {
	//TODO Vérifier les sorties de map
	return (*mapDatas)[newPos.y][newPos.x] == -1 ||
    (*mapDatas)[newPos.y][newPos.x] == 1 ||
    (*mapDatas)[newPos.y][newPos.x] == 2 ||
    (*mapDatas)[newPos.y][newPos.x] == 0;
}

void Character::move (Movement movement, Uint32 time){    
    if(movementAnimationFinished(time)) {
        position_prec = position;
        auto stepped_dir = computeSteppedDirection(orientation);
        if(movement == MOVEMENT_FORWARD /*&& canMove(position+stepped_dir*/)) {
            position += stepped_dir;
        } else if(movement == MOVEMENT_BACKWARD /*&& canMove(position-stepped_dir)*/) {
            position += -stepped_dir;
        } else if(movement == MOVEMENT_LEFT /*&& canMove(position - glm::ivec2(-stepped_dir.y, stepped_dir.x))*/) {
            position += -glm::vec2(-stepped_dir.y, stepped_dir.x);
        } else if(movement == MOVEMENT_RIGHT /*&& canMove(position+glm::ivec2(-stepped_dir.y, stepped_dir.x))*/) {
            position += glm::vec2(-stepped_dir.y, stepped_dir.x);
        }
        
        anim_start_time = time;
    }
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
  cout << "Mesh name : " << mesh_ptr->path <<'\n';

  cout << "******Character - end******" << '\n';
}
