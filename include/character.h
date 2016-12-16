#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include <string>
#include <math.h>
#include <SDL2/SDL.h>
#include "glm.h"

//#include "mesh.h"

enum Movement {
  FORWARD,
  BACK,
  LEFT,
  RIGHT
};

class Character {
public:
  glm::ivec2 position;
  glm::vec3 orientation;
  float anim_start_time, anim_time_normalized;
  float scale;
  std::string id;
  unsigned int life;
  unsigned int defense;
  unsigned int power;
  //Mesh *mesh;

  Character();
  Character(glm::ivec2 position = glm::ivec2(0,0) ,
            glm::vec3 orientation = glm::vec3(0,0,0),
            float scale = 1,
            std::string id = "Default",
            unsigned int life = 0,
            unsigned int defense = 0,
            unsigned int power = 0 );
  ~Character();

  glm::vec3 getVisualPosition();
  void move (Movement movement, Uint32 time);
  void attack (Character* enemy);
  void defend (unsigned int amountAttack);
  void death();

  void print();
};

#endif // CHARACTER_H
