#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include <string>

//#include "mesh.h"

enum Direction {
  NORTH,
  SOUTH,
  EAST,
  WEST
};
enum Movement {
  FORWARD,
  BACK,
  LEFT,
  RIGHT
};

class Character {
private:
  std::string id;
  std::vector<float> position;
  std::vector<float> orientation;
  float scale;
  unsigned int life;
  unsigned int defense;
  unsigned int power;
  //Mesh mesh;

public:
  void move (Movement movement);
  void orient (Direction direction);
  void attack (Character* enemy);
  void defend (unsigned int amountAttack);
};

#endif // CHARACTER_H
