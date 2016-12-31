#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <map>
#include <memory>
#include "glm.h"

#include "mesh.h"
#include "Program.h"
#include "animation.h"

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
  float durability;
  std::shared_ptr<Mesh> mesh_ptr;
  std::shared_ptr<glimac::Program> shaders_ptr;
  std::shared_ptr< std::map<EntityType, std::vector< Animation > > > animations_ptr;

  Item();
  Item(glm::ivec2 position = glm::ivec2(0,0),
  		std::string id = "", int value = 0,
		ItemType type = TREASURE,
		float durability = 2,
		std::shared_ptr<Mesh> mesh_ptr = NULL, std::shared_ptr<glimac::Program> shaders_ptr = NULL);
  ~Item();

  void print();
  bool operator==(const Item & b);

};

#endif //ITEM_H
