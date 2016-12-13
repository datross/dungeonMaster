#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "character.h"
#include "item.h"
#include "camera.h"

enum MenuType {
  INVENTORY,
  MAP,
  OPTIONS,
  QUIT
};

class Item;

class Player : public Character {
private :
  std::vector<Item> inventory;
  unsigned int score;
  Camera cam;

public :
  void openMenu(MenuType type);
  void addToInventory(Item item);
  void equip(Item item);
  void unequip(Item item);
  int isNextDoor();
  void use (Item* item);
};

#endif
