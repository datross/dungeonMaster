#ifndef CHARACTER_H
#define CHARACTER_H

#include <memory>
#include <map>
#include <vector>
#include <string>
#include <math.h>
#include <SDL2/SDL.h>
#include "glm.h"

#include "mesh.h"
#include "animation.h"
#include "Program.h"

enum Movement {
    MOVEMENT_NONE,
    MOVEMENT_FORWARD,
    MOVEMENT_BACKWARD,
    MOVEMENT_LEFT,
    MOVEMENT_RIGHT
};

class Character {
public:
  glm::ivec2 position;

  /* these go together */
  glm::vec3 orientation;

  Uint32 anim_start_time;
  float speed; // inverse du temps d'animation de déplacement d'une case à l'autre
  float scale;
  std::string id;
  unsigned int life;
  unsigned int defense;
  unsigned int power;
  std::shared_ptr<Mesh> mesh_ptr;
  std::shared_ptr<glimac::Program> shaders_ptr;
  std::shared_ptr< std::map <EntityType, std::vector< Animation > > > animations_ptr;

  Character();
  Character(glm::ivec2 position = glm::ivec2(0,0) ,
            glm::vec3 orientation = glm::vec3(0,0,0),
            float scale = 1,
            std::string id = "Default",
            unsigned int life = 0,
            unsigned int defense = 0,
            unsigned int power = 0 ,
			std::shared_ptr<Mesh> mesh_ptr = NULL,
			std::shared_ptr<glimac::Program> shaders_ptr = NULL);
  ~Character();


  void setDatas(std::vector<std::vector<int> >* datas);
  glm::vec3 getVisualPosition(Uint32 time);
  void move (Movement movement, Uint32 time);
  void attack (Character* enemy);
  void defend (unsigned int amountAttack);
  void death();

  void print();
};

#endif // CHARACTER_H
