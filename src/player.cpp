#include "player.h"
#include <algorithm>

using namespace std;

Player::Player(glm::ivec2 position, glm::vec3 orientation,
        	   float scale, string id,
        	   unsigned int life, unsigned int defense, unsigned int power,
               unsigned int score,
			   std::shared_ptr<Mesh> mesh_ptr, std::shared_ptr<glimac::Program> shaders_ptr) :
                    Character(position, orientation, scale, id, life, defense, power, mesh_ptr, shaders_ptr),
                    score(score)
                    {}
Player::~Player(){}

void Player::addItem(Item item){
	std::vector<Item>::iterator i = std::find(inventory.begin(), inventory.end(), item);
	if(i != inventory.end()) {

	} else {
		inventory.push_back(item);
	}
}
void Player::dropItem(Item item){
  inventory.erase(find(inventory.begin(), inventory.end(), item));
}
void Player::equip(Item item){
	use(item);
  	equiped.push_back(item);
}
void Player::unequip(Item item){
	if (item.type == WEAPON) {
	    power -= item.value;
	} else if (item.type == ARMOR) {
	    defense -= item.value;
	}
  	equiped.erase(find(equiped.begin(), equiped.end(), item));
}
int Player::isNextDoor(){
  return -1;
}

void Player::defend (unsigned int amountAttack){
	life -= (amountAttack - defense);
	if(!life) death();
	for(std::vector<Item>::iterator it = equiped.begin(); it!=equiped.end(); ++it){
		if((*it).type == ARMOR){
			(*it).durability-= 0.05f;
			if((*it).durability < 0){
				unequip((*it));
				dropItem((*it));
			} else if ((*it).durability > 1) {
				(*it).durability = 2;
			}
		}
	}
}
void Player::attack (Character* enemy){
	enemy->defend(power);
	for(std::vector<Item>::iterator it = equiped.begin(); it!=equiped.end(); ++it){
		if((*it).type == WEAPON){
			(*it).durability-= 0.1f;
			if((*it).durability < 0){
				unequip((*it));
				dropItem((*it));
			} else if ((*it).durability > 1) {
				(*it).durability = 2;
			}
		}
	}
}

void Player::use(Item item){
	if (item.type == TREASURE) {
	    score += item.value;
	} else if (item.type == WEAPON) {
	    power += item.value;
	} else if (item.type == ARMOR) {
	    defense += item.value;
	} else if (item.type == HEALTH) {
	    life += item.value;
	} else if (item.type == KEY) {
		if(isNextDoor() == item.value) {

		}
  }
}

void Player::updateCam(glm::ivec2 mouseMotion) {
    cam.position = glm::vec3(position.x, 0.7, -position.y);
    cam.rotation.x += mouseMotion.x;
    cam.rotation.y -= mouseMotion.y;
    if(cam.rotation.y > 90) cam.rotation.y = 90;
    else if(cam.rotation.y < -90) cam.rotation.y = -90;
}
