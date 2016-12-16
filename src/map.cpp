#include "map.h"

#include <fstream>
#include <iostream>
#include <algorithm>

#include "enemy.h"

using namespace std;

Map::Map(){}
Map::~Map(){}

void Map::save(string pathFile){
	ofstream file;
	file.open(pathFile, ios::out | ios::trunc);

	if(file){
		file << name << endl;
		file << players.size() << endl;
		for (list<Player>::iterator it = players.begin(); it != players.end(); ++it) {
			file << (*it).position.x << " " << (*it).position.y << " " << (*it).scale << " " << (*it).id;
			file << " " << (*it).life << " " << (*it).defense << " " << (*it).power;
			file << " " << (*it).inventory.size()+(*it).equiped.size();
			file << " " << (*it).score << " " << /*(*it).mesh.path << */ endl;

			for (vector<Item>::iterator item = (*it).inventory.begin(); item != (*it).inventory.end(); ++item) {
				file << '\t' << (*item).id << " " << (*item).value << " " << (*item).type;
				file << " " << (*item).durability << " " << 0 << " " << /*(*it).mesh.path << */ endl;
			}
		}

		file << items.size() << endl;
		for (list<Item>::iterator it = items.begin(); it != items.end(); ++it) {
			file << (*it).position.x << " " << (*it).position.y << " " << (*it).id;
			file << " " << (*it).value << " " << (*it).type << " " << (*it).durability;
			file << " " << /*(*it).mesh.path << */ endl;
		}

		file << characters.size() << endl;
		for (list<Enemy>::iterator it = characters.begin(); it != characters.end(); ++it) {
			file << (*it).position.x << " " << (*it).position.y << " " << (*it).scale << " " << (*it).id;
			file << " " << (*it).life << " " << (*it).defense << " " << (*it).power;
			file << " " << (*it).detectRange;
			file << " " << /*(*it).mesh.path << */ endl;
		}

		file << traps.size() << endl;
		for (vector<Trap>::iterator it = traps.begin(); it != traps.end(); ++it) {
			file << (*it).position.x << " " << (*it).position.y << " " << (*it).id;
			file << " " << (*it).damages << " " << (*it).activationTiming;
			file << " " << /*(*it).mesh.path << */ endl;
		}

	} else
		cerr << "Cannot open or create " << pathFile << endl;

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

    unsigned int nb_Player, nb_Item, nb_Enemy, nb_Traps;
    string line;
    string delimiter = ":";
    if(file){

        getline(file, line);
        name = line;

        int posX, posY, value, value_inventory, type, type_inventory, durability, durability_inventory;
        unsigned int life, defense, power, detectRange, damages, timing, nb_items_inventory, score, isEquiped;
        string id, id_inventory, mesh_path;
        float scale;

        file >> nb_Player;
        for (unsigned int i = 0; i < nb_Player; i++) {
            file >> posX >> posY >> scale >>  id >>  life >>  defense >>  power >> nb_items_inventory >> score >> mesh_path;
            getline(file, line);
            glm::ivec2 position = glm::ivec2(posX, posY);
            Player tmp_player = Player(position, glm::vec3(0,0,0), scale, id, life, defense, power, score);

            for (unsigned int j = 0; j < nb_items_inventory; j++) {
                file >> id_inventory >>  value_inventory >> type_inventory >>  durability_inventory >> isEquiped >> mesh_path;
                getline(file, line);
                position = glm::ivec2(0, 0);
                if(isEquiped)
                    tmp_player.equip(Item(position, id_inventory, value_inventory, (ItemType)type_inventory, durability_inventory));
                else
                    tmp_player.addItem(Item(position, id_inventory, value_inventory, (ItemType)type_inventory, durability_inventory));
            }
            players.push_back(tmp_player);
        }

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
         * for (unsigned int i = 0; i < datas.size(); i++) {
		for (unsigned int j = 0; j < datas[0].size(); j++) {
			cout << distances[i][j] << " ";
		}
		cout << endl;
	}*/

    return distances;
}
