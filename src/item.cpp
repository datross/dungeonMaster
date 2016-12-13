#include "item.h"

Item::Item(){}
Item::Item(std::string id, int value, ItemType type, int durability) :
  id(id),
  value(value),
  type(type),
  durability(durability)
  {}
Item::~Item(){}
