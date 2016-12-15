#include "item.h"

#include <iostream>

using namespace std;

Item::Item(){}
Item::Item(glm::ivec2 position, std::string id, int value, ItemType type, int durability) :
  position(position),
  id(id),
  value(value),
  type(type),
  durability(durability)
  {}
Item::~Item(){}

std::string Item::getId() const{
  return id;
}
int Item::getValue() const{
  return value;
}
ItemType Item::getType() const{
  return type;
}
int Item::getDurability() const{
  return durability;
}

void Item::print(){
  cout << "******Item******" << '\n';
  cout << "Id : " << id << endl;
  cout << "position : (" << position[0] << ";" << position[1] << ")" <<'\n';
  cout << "Type/value/durability : (" << type << ";" << value <<";" << durability <<")" <<'\n';
  cout << "******Item - end******" << '\n';
}
bool Item::operator==(const Item & b){
  if(!id.compare(b.getId())){
    if(value == b.getValue()) {
      if(type == b.getType()){
        if(durability == b.getDurability())
          return true;
      }
    }
  }
  return false;
}
