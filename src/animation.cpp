#include "animation.h"
#include <fstream>
#include <iostream>
using namespace std;

void Animation::loadAnimation(std::string path){
	ifstream file;
    file.open(path, ios::in);

    if(file){
		string line;
		unsigned int steps;
		file >> time >> steps;
		getline(file, line);
		getline(file, line);

		for (size_t i = 0; i < steps; i++) {
			glm::vec3 tmp_position, tmp_rotation, tmp_scale;
			file >> anim.first >> tmp_position.x >> tmp_position.y >> tmp_position.z;
			file >> tmp_rotation.x >> tmp_rotation.y >> tmp_rotation.z;
			file >> tmp_scale.x >> tmp_scale.y >> tmp_scale.z;

			getline(file, line);
		}

        file.close();
    } else
        cerr << "Cannot open animation " << path << endl;
}

void Animation::execute(){
	return;
}
