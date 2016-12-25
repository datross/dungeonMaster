#include "animation.h"
#include <fstream>
#include <iostream>
using namespace std;

Animation::Animation(){}
Animation::~Animation(){}
Animation::Animation(std::string path){
	loadAnimation(path);
}

void Animation::loadAnimation(std::string path){
	ifstream file;
    file.open(path, ios::in);

    if(file){
		string line;
		unsigned int nb_steps;
		file >> duration >> nb_steps;

		for (size_t i = 0; i < nb_steps; i++) {
			pair <float, vector<glm::vec3> > step;

			glm::vec3 tmp_position = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 tmp_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 tmp_scale = glm::vec3(0.0f, 0.0f, 0.0f);
			file >> step.first >> tmp_position[0] >> tmp_position[1] >> tmp_position[2];
			file >> tmp_rotation[0] >> tmp_rotation[1] >> tmp_rotation[2];
			file >> tmp_scale[0] >> tmp_scale[1] >> tmp_scale[2];
			step.second.push_back(tmp_position);
			step.second.push_back(tmp_rotation);
			step.second.push_back(tmp_scale);

			anim.push_back(step);
		}

        file.close();
    } else
        cerr << "Cannot open animation " << path << endl;
}

void Animation::execute(){
	return;
}
