#include "map.h"

#include <fstream>
#include <iostream>

#include "enemy.h"

using namespace std;

Map::Map(){}
Map::~Map(){}

void Map::save(string pathFile){

}

void Map::loadTerrain(string pathFile){

  unsigned int width;
  unsigned int height;
  unsigned int maxValue;

  ifstream file;
  file.open(pathFile, ios::in);

  if(file){
    string line, line2, line3;
    getline(file, line);
    if(line.compare("P3")!=0){
      cerr << "Wrong format for " << pathFile << endl;
      return;
    }
    getline(file, line);
    file >> width >> height >> maxValue;
    getline(file, line);
    vector< vector<int> > F_datas (height, vector<int>(width));

    unsigned int i = 0,j = 0, nb_door = 0;
    while(getline(file, line) && j*width + i < width*height){
      getline(file, line2);
      getline(file, line3);

      if(!line.compare("0") && !line2.compare("0") && !line3.compare("255")){
        F_datas[j][i] = -3;   // WATER
      } else if(!line.compare("0") && !line2.compare("0") && !line3.compare("0")){
        F_datas[j][i] = -2;   // WALL
      } else if(!line.compare("255") && !line2.compare("255") && !line3.compare("255")){
        F_datas[j][i] = 0;    // FREE
      } else if(!line.compare("255") && !line2.compare("0") && !line3.compare("0")){
        F_datas[j][i] = 1;    //ENTRY
      } else if(!line.compare("0") && !line2.compare("255") && !line3.compare("0")){
        F_datas[j][i] = 2;    //EXIT
      } else if(!line.compare("170") && !line2.compare("119") && !line3.compare("34")){
        F_datas[j][i] = 3 + nb_door; //DOORS
        nb_door++;
      }

      ++i;
      if(i>=width){
        i=0;
        ++j;
      }
    }
    datas = F_datas;
    file.close();
  } else
    cerr << "Cannot open " << pathFile << endl;
}

void Map::loadEntities(string pathFile){
  ifstream file;
  file.open(pathFile, ios::in);

  unsigned int nb_Item, nb_Enemy, nb_Traps;
  string line;
  string delimiter = ":";
  if(file){

    getline(file, line);
    name = line;

    int posX, posY, value, type, durability;
    unsigned int life, defense, power, detectRange, damages, timing;
    string id, mesh_path;
    float scale;

    file >> nb_Item;
    for (unsigned int i = 0; i < nb_Item; i++) {
      file >> posX >> posY >> id >>  value >>  type >>  durability >>  mesh_path;
      getline(file, line);
      glm::ivec2 position = glm::ivec2(posX, posY);
      items.push_back(Item(position, id, value, (ItemType)type, durability));
    }

    file >> nb_Enemy;
    for (unsigned int i = 0; i < nb_Enemy; i++) {
      file >> posX >> posY >> scale >>  id >>  life >>  defense >>  power >>  detectRange >>  mesh_path;
      getline(file, line);
      glm::ivec2 position = glm::ivec2(posX, posY);
      characters.push_back(Enemy(position, glm::vec3(0,0,0), scale, id, life, defense, power, detectRange));
    }

    file >> nb_Traps;
    for (unsigned int i = 0; i < nb_Traps; i++) {
      file >> posX >> posY >> id >>  damages >>  timing >>  mesh_path;
      getline(file, line);
      glm::ivec2 position = glm::ivec2(posX, posY);
      traps.push_back(Trap(position, id, damages, timing));
    }

    file.close();
  } else
    cerr << "Cannot open " << pathFile << endl;
}


bool Map::isCaseEmpty(int x, int y){
	return datas[x][y] == 0;
}

void Map::load(string file_name){
  string path = "res/levels/";

  loadTerrain(path + file_name + ".ppm");
  loadEntities(path + file_name + ".txt");
}


void Map::print() {
  std::cout << "MAP : " << endl;
  std::cout << "/* datas : */" << endl;
  std::cout << "***********************************" << '\n';
  for (std::vector< std::vector<int> >::iterator it = datas.begin() ; it != datas.end(); ++it) {
    cout << "|";
    for (std::vector<int>::iterator it_bis = (*it).begin() ; it_bis != (*it).end(); ++it_bis) {
      cout << *it_bis << "; ";
    }
    cout << "|" << endl;
  }
  std::cout << "***********************************" << '\n';
  std::cout << "/* Characters : */" << endl;
  for (std::list<Character>::iterator it=characters.begin(); it != characters.end(); ++it) {
   it->print();
  }
  std::cout << "/* Items : */" << endl;
  for (std::list<Item>::iterator it=items.begin(); it != items.end(); ++it) {
   it->print();
  }
  std::cout << "/* Traps : */" << endl;
  for (std::vector<Trap>::iterator it=traps.begin(); it != traps.end(); ++it) {
   it->print();
  }

  std::cout << "MAP END." << endl;
}