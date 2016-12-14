#include "item.h"

#include <iostream>

using namespace std;

Item::Item(){}
Item::Item(std::string id, int value, ItemType type, int durability) :
  id(id),
  value(value),
  type(type),
  durability(durability)
  {}
Item::~Item(){}

void Item::print(){
  cout << "******Item******" << '\n';
  cout << "Id : " << id << endl;
  cout << "position : (" << position[0] << ";" << position[1] << ")" <<'\n';
  cout << "Type/value/durability : (" << type << ";" << value <<";" << durability <<")" <<'\n';
  cout << "******Item - end******" << '\n';
}
