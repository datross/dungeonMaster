#include "trap.h"

#include <iostream>

using namespace std;

Trap::Trap(){}
Trap::Trap(glm::ivec2 position, std::string id,
		   int damages, unsigned int timing,
		   shared_ptr<Mesh> mesh_ptr, shared_ptr<glimac::Program> shaders_ptr) :
    			position(position), id(id),
				damages(damages), activationTiming(timing),
				mesh_ptr(mesh_ptr), shaders_ptr(shaders_ptr)
				{}
Trap::~Trap(){}

void Trap::activate(){}
void Trap::reset(){}
bool Trap::detect(){
  return false;
}

void Trap::print(){
  cout << "******Trap******" << '\n';
  cout << "Id : " << id << endl;
  cout << "position : (" << position[0] << ";" << position[1] << ")" <<'\n';
  cout << "damage/timing : (" << damages << ";" << activationTiming <<")" <<'\n';
  cout << "******Trap - end******" << '\n';
}
