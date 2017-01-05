#include "animation.h"
#include <fstream>
#include <iostream>

using namespace std;

Animation::Animation(){}
Animation::~Animation(){}
Animation::Animation(std::string path){
	loadAnimation(path);
	beginning = 0;
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

template <typename T>
bool Animation::execute(T& entity){
	bool isInProgress = false;
	Uint32 ticks = SDL_GetTicks();
	if(beginning == 0) {
		beginning = ticks;
		isInProgress = true;
	}
	float animProgress = (ticks - beginning)/duration;
	if(animProgress >= 1){
		beginning = 0;
		isInProgress = false;
	} else {
		for (std::vector<std::pair< float , std::vector< glm::vec3 > > >::iterator it = anim.begin();
			 it != anim.end()-1;
		 	 it++) {
				 if(animProgress >= (*it).first && animProgress < (*(++it)).first) {
					 float interpolValue = (animProgress- (*it).first)/((*(++it)).first - (*it).first);
					 entity.position += interpolValue * ((*it).second[0]-(*(++it)).second[0]);
					 entity.orientation += interpolValue * ((*it).second[1]/(*(++it)).second[1]);
				 }
		}
	}
	return isInProgress;
}

void Animation::print(){
	std::cout << "Duration : " << duration << '\n';

	for (std::vector<std::pair< float , std::vector< glm::vec3 > > >::iterator it = anim.begin();
		 it != anim.end();
		 ++it){
		 	std::cout << (*it).first * 100 << "% ";
			for (std::vector< glm::vec3 >::iterator it2 = (*it).second.begin();
				 it2 != (*it).second.end();
				 ++it2){
					 std::cout << "(" << (*it2).x << "," << (*it2).y << "," << (*it2).z << ")";
			}
			std::cout << '\n';
	}
}
