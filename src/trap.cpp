#include "trap.h"

#include <iostream>

using namespace std;

Trap::Trap(){}
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
  cout << "damage/timing : (" << damage << ";" << activationTiming <<")" <<'\n';
  cout << "******Trap - end******" << '\n';
}
