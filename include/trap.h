#ifndef TRAP_H
#define TRAP_H

#include "glm.h"
#include <string>
#include <map>
#include <memory>

#include "mesh.h"
#include "Program.h"
#include "animation.h"

class Trap {
public:
  glm::ivec2 position;
  glm::vec3 orientation;
  std::string id;
  int damages;
  unsigned int activationTiming;
  Mesh * mesh_ptr;
  glimac::Program * shaders_ptr;
  std::map<EntityType, std::vector< Animation > > * animations_ptr;

  Trap();
  Trap(glm::ivec2 position = glm::ivec2(0,0),
  std::string id = "Default", int damages = 0,
  unsigned int timing = 1000,
  Mesh * mesh_ptr = NULL, glimac::Program * shaders_ptr = NULL);
  ~Trap();

  void activate();
  void reset();
  bool detect();

  void print();
};

#endif //TRAP_H
