#include "item.h"

#include <iostream>

using namespace std;

Item::Item(){}
Item::Item(glm::ivec2 position, std::string id,
		   int value, ItemType type, int durability,
		   std::shared_ptr<Mesh> mesh_ptr, std::shared_ptr<glimac::Program> shaders_ptr) :
			  position(position), id(id),
			  value(value), type(type), durability(durability),
			  mesh_ptr(mesh_ptr), shaders_ptr(shaders_ptr)
			  {}
Item::~Item(){}

void Item::print(){
  cout << "******Item******" << '\n';
  cout << "Id : " << id << endl;
  cout << "position : (" << position[0] << ";" << position[1] << ")" <<'\n';
  cout << "Type/value/durability : (" << type << ";" << value <<";" << durability <<")" <<'\n';
  cout << "******Item - end******" << '\n';
}
bool Item::operator==(const Item & b){
  if(!id.compare(b.id)){
    if(value == b.value) {
      if(type == b.type){
        if(durability == b.durability)
          return true;
      }
    }
  }
  return false;
}
