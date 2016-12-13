#include "map.h"

#include <fstream>
#include <iostream>

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
  file.open("res/levels/" + pathFile, ios::in);
  cout << "res/levels/" + pathFile << endl;
  if(file){
    string line, line2, line3;
    getline(file, line);
    if(line.compare("P3")!=0){
      cerr << "Wrong format for " << pathFile << endl;
      return;
    }
    getline(file, line);
    file >> width >> height >> maxValue;

    vector< vector<int> > F_datas (height, vector<int>(width));

    unsigned int i = 0,j = 0, nb_door = 0;
    while(getline(file, line) && j*width + i < width*height){
      getline(file, line2);
      getline(file, line3);

      cout << i << " " << j << endl;

      if(line.compare("0") && line2.compare("0") && line3.compare("255")){
        F_datas[j][i] = -3;   // WATER
      } else if(line.compare("0") && line2.compare("0") && line3.compare("0")){
        F_datas[j][i] = -2;   // WALL
      } else if(line.compare("255") && line2.compare("255") && line3.compare("255")){
        F_datas[j][i] = 0;    // FREE
      } else if(line.compare("255") && line2.compare("0") && line3.compare("0")){
        F_datas[j][i] = 1;    //ENTRY
      } else if(line.compare("0") && line2.compare("255") && line3.compare("0")){
        F_datas[j][i] = 2;    //EXIT
      } else if(line.compare("170") && line2.compare("119") && line3.compare("34")){
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

}
bool Map::isCaseEmpty(){
  return false;
}
