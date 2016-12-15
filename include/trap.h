#ifndef TRAP_H
#define TRAP_H

#include "glm.h"
#include <string>

//#include "mesh.h"

class Trap {
public:
  glm::ivec2 position;
  glm::vec3 orientation;
  std::string id;
  int damages;
  unsigned int activationTiming;
  //Mesh mesh

  Trap();
  Trap(glm::ivec2 position = glm::ivec2(0,0), std::string id = "Default", int damages = 0, unsigned int timing = 1000);
  ~Trap();

  void activate();
  void reset();
  bool detect();

  void print();
};

#endif //TRAP_H
