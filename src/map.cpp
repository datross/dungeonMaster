#include "map.h"

#include <fstream>
#include <iostream>
#include <algorithm>

#include "enemy.h"
#include "mesh.h"

using namespace std;

Map::Map(){}

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

bool Map::isCaseEmpty(int x, int y){
    return datas[x][y] == 0 || datas[x][y] == 1 || datas[x][y] == 2;
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
    std::cout << "/* Players : */" << endl;
    for (std::list<Player>::iterator it=players.begin(); it != players.end(); ++it) {
        it->print();
    }
    std::cout << "/* Characters : */" << endl;
    for (std::list<Enemy>::iterator it=characters.begin(); it != characters.end(); ++it) {
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

bool Map::isCaseAccessible(int x, int y) {
    return datas[y][x] == -1 ||
    datas[y][x] == 1 ||
    datas[y][x] == 2 ||
    datas[y][x] == 0;
}

int findMin(std::vector<int> q, std::vector<int> dist, int ordre) {
	int min = 255;
	int sommet = -1;

	std::vector<int>::iterator it;

	int i;
	for (i = 0; i < ordre; i++) {
		it = find(q.begin(), q.end(), i);
		if (it == q.end()) {
			if (dist[i] < min) {
				min = dist[i];
				sommet = i;
			}
		}
	}

	return sommet;
}

bool isVoisin(glm::ivec2 source, glm::ivec2 tested) {
	if (glm::ivec2(tested.x+1, tested.y) == source)
		return true;
	if (glm::ivec2(tested.x-1, tested.y) == source)
		return true;
	if (glm::ivec2(tested.x, tested.y+1) == source)
		return true;
	if (glm::ivec2(tested.x, tested.y-1) == source)
		return true;
	return false;
}

void Map::clear(){
	name = "";
	datas.clear();
	players.clear();
	characters.clear();
	items.clear();
	traps.clear();
}

std::vector<std::vector<unsigned int>> Map::getDistance(int numPlayer) {
    std::vector<glm::ivec2> link;
    unsigned int nbCorridors = 0;
    unsigned int sourceIndex;

	// Attributing node number to coordinates
    for (unsigned int i = 0; i < datas.size(); i++) {
        for (unsigned int j = 0; j < datas[0].size(); j++) {
            // Get the player index
			list<Player>::iterator pit = players.begin();
			for (int i = 0; i < numPlayer; i++) {
					++pit;
			}
            if ((unsigned int) pit->position.x == j && (unsigned int) pit->position.y == i) {
                sourceIndex = nbCorridors;
            }

            if (isCaseAccessible(j,i)) {
                link.push_back(glm::ivec2(i, j));
                nbCorridors++;
            }
        }
    }

    // Initialisation
    std::vector<int> dist;
    for (unsigned int i = 0; i < nbCorridors; i++) {
        if (i == sourceIndex)
            dist.push_back(0);
        else
            dist.push_back(255);
    }

    std::vector<int> q;
    while (q.size() < nbCorridors) {
        int s = findMin(q, dist, nbCorridors);
        q.push_back(s);
        for (unsigned int i = 0; i < nbCorridors; i++) {
            if (isVoisin(link[s], link[i])) {
                if (dist[i] > dist[s] + 1) {
                    dist[i] = dist[s]+1;
                }
            }
        }
    }

    // Creation of the distance matrix, linking between coordinates and minimal distances
    std::vector<std::vector<unsigned int>> distances(datas.size(), vector<unsigned int>(datas[0].size()));
	for (unsigned int i = 0; i < datas.size(); i++) {
		for (unsigned int j = 0; j < datas[0].size(); j++) {
			distances[i][j] = 255;
		}
	}

	for (unsigned int i = 0; i < nbCorridors; i++) {
		distances[link[i].x][link[i].y] = dist[i];
	}

	/* Affichage pour debeug
          for (unsigned int i = 0; i < datas.size(); i++) {
		for (unsigned int j = 0; j < datas[0].size(); j++) {
			cout << distances[i][j] << " ";
		}
		cout << endl;
	}*/

    return distances;
}
