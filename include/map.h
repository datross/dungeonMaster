#ifndef MAP_H
#define MAP_H

#include "glm.h"
#include <math.h>
#include <vector>
#include <list>
#include <string>

#include "player.h"
#include "enemy.h"
#include "item.h"
#include "trap.h"

class Map {
public:

  std::string name;
  std::vector< std::vector<int> > datas;
  std::list<Player> players;
  std::list<Enemy> characters;
  std::list<Item> items;
  std::vector<Trap> traps;

  Map();
  ~Map();

  void loadTerrain(std::string pathFile);
  bool isCaseEmpty(int x, int y);
  bool isCaseAccessible(int x, int y);
  std::vector<std::vector<unsigned int>> getDistance(int numPlayer);

  void print();
};

#endif // MAP_H
