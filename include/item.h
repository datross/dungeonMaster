#ifndef ITEM_H
#define ITEM_H

#include <string>

//#include "mesh.h"
#include "character.h"

enum ItemType {
  TREASURE = 0,
  WEAPON = 1,
  ARMOR = 2,
  HEALTH = 3,
  KEY = 4
};

class Item {
private:
  glm::ivec2 position;
  glm::vec3 orientation;
  std::string id;
  int value;
  ItemType type;
  int durability;
  //Mesh mesh;

public:
  Item();
  Item(glm::ivec2 position = glm::ivec2(0,0), std::string id = "", int value = 0, ItemType type = TREASURE, int durability = -1);
  ~Item();

  void print();

};

#endif //ITEM_H
