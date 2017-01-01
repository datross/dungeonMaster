#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>

#include "character.h"
#include "item.h"
#include "camera.h"

enum MenuType {
  INVENTORY,
  MAP,
  OPTIONS,
  QUIT
};

class Player : public Character {
public :
  std::vector<Item> inventory;
  std::vector<Item> equiped;
  unsigned int score;
  Camera cam;

  Player(glm::ivec2 position = glm::ivec2(0,0), glm::vec3 orientation = glm::vec3(0,0,0),
        float scale = 1, std::string id = "Default",
        unsigned int life = 0, unsigned int defense = 0, unsigned int power = 0,
        unsigned int score = 0,
		std::shared_ptr<Mesh> mesh_ptr = NULL, std::shared_ptr<glimac::Program> shaders_ptr = NULL);
  ~Player();

  void addItem(Item item);
  void dropItem(Item item); 
  void equip(Item item);
  void unequip(Item item);
  int isNextDoor();
  void use (Item item);
  void attack (Character* enemy);
  void defend (unsigned int amountAttack);
};

#endif
