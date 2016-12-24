#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <memory>
#include "glm.h"

#include "mesh.h"
#include "Program.h"

enum ItemType {
  TREASURE = 0,
  WEAPON = 1,
  ARMOR = 2,
  HEALTH = 3,
  KEY = 4
};

class Item {
public:
  glm::ivec2 position;
  glm::vec3 orientation;
  std::string id;
  int value;
  ItemType type;
  int durability;
  std::shared_ptr<Mesh> mesh_ptr;
  std::shared_ptr<glimac::Program> shaders_ptr;

  Item();
  Item(glm::ivec2 position = glm::ivec2(0,0),
  		std::string id = "", int value = 0,
		ItemType type = TREASURE,
		int durability = -1,
		std::shared_ptr<Mesh> mesh_ptr = NULL, std::shared_ptr<glimac::Program> shaders_ptr = NULL);
  ~Item();

  void print();
  bool operator==(const Item & b);

};

#endif //ITEM_H
