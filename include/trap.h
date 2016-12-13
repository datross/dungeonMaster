#ifndef TRAP_H
#define TRAP_H

#include "glm.h"
#include <string>

//#include "mesh.h"

class Trap {
private:
  glm::ivec2 position;
  glm::vec3 orientation;
  std::string id;
  int damage;
  uint activationTiming;
  //Mesh mesh

public:
  Trap();
  ~Trap();
  
  void activate();
  void reset();
  bool detect();
};

#endif //TRAP_H
