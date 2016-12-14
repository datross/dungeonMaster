#include "player.h"
#include <algorithm>

using namespace std;

Player::Player(glm::ivec2 position, glm::vec3 orientation,
            float scale, string id,
            unsigned int life, unsigned int defense, unsigned int power,
             unsigned int score) :
                    Character(position, orientation, scale, id, life, defense, power),
                    score(score)
                    {}
Player::~Player(){}

std::vector<Item> Player::getInventory(){
  return inventory;
}
std::vector<Item> Player::getStuff(){
  return equiped;
}
void Player::setInventory(std::vector<Item> inv){
  inventory = inv;
}
void Player::setStuff(std::vector<Item> inv){
  equiped = inv;
}

void Player::openMenu(MenuType type){
  return;
}
void Player::addItem(Item item){
  inventory.push_back(item);
}
void Player::dropItem(Item item){
  inventory.erase(find(inventory.begin(), inventory.end(), item));
}
void Player::equip(Item item){
  equiped.push_back(item);
}
void Player::unequip(Item item){
  equiped.erase(find(equiped.begin(), equiped.end(), item));
}
int Player::isNextDoor(){
  return -1;
}

void Player::use(Item* item){
  /*
  if (type == TREASURE) {
    player->score += value;
  } else if (type == WEAPON) {
    palyer->power += value;
  } else if (type == ARMOR) {
    player->defense += value;
  } else if (type == HEALTH) {
    player->life += value;
  } else if (type == KEY) {
    int door = player->isNextDoor();
    if(door >= 3 && value == door) {
      //Open door
    }
  }
  */
}
