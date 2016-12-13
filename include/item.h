#ifndef ITEM_H
#define ITEM_H

#include <string>

//#include "mesh.h"
#include "character.h"

enum ItemType {
  TREASURE,
  WEAPON,
  ARMOR,
  HEALTH,
  KEY
};

class Item {
private:
  std::string id;
  int value;
  ItemType type;
  int durability;
  //Mesh mesh;

public:
  Item();
  Item(std::string id = "", int value = 0, ItemType type = TREASURE, int durability = -1);
  ~Item();

};

#endif //ITEM_H
