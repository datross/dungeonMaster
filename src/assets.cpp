#include "assets.h"

#include <fstream>
#include <iostream>
#include <utility>
#include <memory>

using namespace std;

Assets::Assets(char* _application_path): application_path(_application_path){
	application_path = application_path.dirPath() + "../";
}
Assets::~Assets(){}


Mesh * Assets::meshLoad(glimac::FilePath mesh_path){
    /* check if no double */
    for(vector<Mesh>::iterator it = meshes.begin(); it != meshes.end(); ++it){
        if((*it).path == mesh_path){
            return &(*it);
        }
    }
    /* else add the new mesh to assets and return ptr */
    Mesh * mesh = new Mesh();
    mesh->path = mesh_path;
    meshes.push_back(*mesh);
    return mesh;
}

glimac::Program * Assets::shadersLoad(glimac::FilePath vShader_path, glimac::FilePath fShader_path){
    /* check if no double */
    for(vector<glimac::Program>::iterator it = shaders.begin(); it != shaders.end(); ++it){
        if((*it).vertexShaderPath.compare(vShader_path) == 0 &&
		   (*it).fragmentShaderPath.compare(fShader_path) == 0){
			   return &(*it);
        }
    }

    /* else add the new program to assets and return ptr */
    glimac::Program * shader = new glimac::Program();
    (*shader) = glimac::loadProgram(application_path + SHADER_PATH + vShader_path, application_path + SHADER_PATH + fShader_path);
    shader->vertexShaderPath = vShader_path;
    shader->fragmentShaderPath = fShader_path;
    shaders.push_back(*shader);
    return shader;
}

void Assets::load(string fileName, bool isNewGame){
    /* load map */
    map.loadTerrain(application_path + DATA_PATH + (fileName + ".ppm"));

    /* load entities */
    if(isNewGame) loadEntities(application_path + DATA_PATH + (fileName + ".txt"));
    else loadEntities(application_path + SAVE_PATH + (fileName + ".txt"));

    /* load meshes */
    for(vector<Mesh>::iterator it = meshes.begin(); it != meshes.end(); ++it){
        (*it).loadFromFile(application_path + MESH_PATH + (*it).path);
    }
    wall.loadFromFile("res/meshes/cube.obj");
    ground.loadFromFile("res/meshes/ground.obj");
    ceiling.loadFromFile("res/meshes/ceiling.obj");
    water.loadFromFile("res/meshes/water.obj");

    /* load animations */
    std::vector< std::pair<EntityType, std::string> > animsPacks;
    animsPacks.push_back(std::pair<EntityType, std::string>(DROPABLE_ITEM,"item_pack.txt"));

    for(std::vector<std::pair<EntityType, std::string> >::iterator it=animsPacks.begin();
            it != animsPacks.end(); ++it){
                    loadAnimationsPack((*it).first, application_path + ANIMATION_PATH + (*it).second);
    }

    /* load general shader */
    generalShader = std::shared_ptr<glimac::ProgramWrapper>(new glimac::ProgramWrapper("res/shaders/3D.vs.glsl",
                    "res/shaders/pointlight.fs.glsl"));

    print();
}

void Assets::save(glimac::FilePath fileName){
	glimac::FilePath filePath = application_path + SAVE_PATH + fileName;
    ofstream file;
    file.open(filePath, ios::out | ios::trunc);

    if(file){
        file << map.name << endl;
        file << map.players.size() << endl;
        for (list<Player>::iterator it = map.players.begin(); it != map.players.end(); ++it) {
            file << (*it).position.x << " " << (*it).position.y << " " << (*it).scale << " " << (*it).id;
            file << " " << (*it).life << " " << (*it).defense << " " << (*it).power;
            file << " " << (*it).inventory.size()+(*it).equiped.size();
            file << " " << (*it).score << " " << (*it).mesh_ptr->path << " " << (*it).shaders_ptr->vertexShaderPath << " " << (*it).shaders_ptr->fragmentShaderPath << endl;

            for (vector<Item>::iterator item = (*it).inventory.begin(); item != (*it).inventory.end(); ++item) {
                file << '\t' << (*item).id << " " << (*item).value << " " << (*item).type;
                file << " " << (*item).durability << " " << 0 << " " << (*it).mesh_ptr->path << " " << (*it).shaders_ptr->vertexShaderPath << " " << (*it).shaders_ptr->fragmentShaderPath << endl;
            }
        }

        file << map.items.size() << endl;
        for (list<Item>::iterator it = map.items.begin(); it != map.items.end(); ++it) {
            file << (*it).position.x << " " << (*it).position.y << " " << (*it).id;
            file << " " << (*it).value << " " << (*it).type << " " << (*it).durability;
            file << " " << (*it).mesh_ptr->path << " " << (*it).shaders_ptr->vertexShaderPath << " " << (*it).shaders_ptr->fragmentShaderPath << endl;
        }

        file << map.characters.size() << endl;
        for (list<Enemy>::iterator it = map.characters.begin(); it != map.characters.end(); ++it) {
            file << (*it).position.x << " " << (*it).position.y << " " << (*it).scale << " " << (*it).id;
            file << " " << (*it).life << " " << (*it).defense << " " << (*it).power;
            file << " " << (*it).detectRange;
            file << " " << (*it).mesh_ptr->path << " " << (*it).shaders_ptr->vertexShaderPath << " " << (*it).shaders_ptr->fragmentShaderPath << endl;
        }

        file << map.traps.size() << endl;
        for (vector<Trap>::iterator it = map.traps.begin(); it != map.traps.end(); ++it) {
            file << (*it).position.x << " " << (*it).position.y << " " << (*it).id;
            file << " " << (*it).damages << " " << (*it).activationTiming;
            file << " " << (*it).mesh_ptr->path << " " << (*it).shaders_ptr->vertexShaderPath << " " << (*it).shaders_ptr->fragmentShaderPath << endl;
        }

    } else
        cerr << "Cannot open or create " << filePath << endl;

}

void Assets::loadEntities(glimac::FilePath pathFile){

    ifstream file;
    file.open(pathFile, ios::in);

    unsigned int nb_Player, nb_Item, nb_Enemy, nb_Traps;
    string line;
    if(file){

        /* Level name */
        getline(file, line);
        map.name = line;

		//Datas container variables, to create objects
        int posX, posY, value, value_inventory, type, type_inventory;
        unsigned int life, defense, power, detectRange, damages, timing, nb_items_inventory, score, isEquiped;
        string id, id_inventory, mesh_path, vShader_path, fShader_path;
        float scale, durability, durability_inventory;
        Mesh * mesh_ptr;
        glimac::Program * shaders_ptr;
		std::map<EntityType, std::vector<Animation> > * animationsLink = &animations;


		/* ---- Players initialization ---- */

	/* Number of player */
        file >> nb_Player;

        for (unsigned int i = 0; i < nb_Player; i++) {
            /* Player infos in file */
            file >> posX >> posY >> scale >>  id >>  life >>  defense >>  power >> nb_items_inventory >> score >> mesh_path >> vShader_path >> fShader_path;
            getline(file, line);

            /* init position */
            glm::ivec2 position = glm::ivec2(posX, posY);

            /* check pointers */
            mesh_ptr = meshLoad(mesh_path);
			shaders_ptr = shadersLoad(vShader_path, fShader_path);

            /* Player creation */
            Player tmp_player = Player(position, glm::vec3(0,0,0), scale, id, life, defense, power, score, mesh_ptr, shaders_ptr);
			/* Link to animations */
			tmp_player.animations_ptr = animationsLink;
			tmp_player.setMap(&map);

			/*  Create his Inventory */
            for (unsigned int j = 0; j < nb_items_inventory; j++) {

				/*  Item infos in  file */
                file >> id_inventory >>  value_inventory >> type_inventory >>  durability_inventory >> isEquiped >> mesh_path >> vShader_path >> fShader_path;
                getline(file, line);

				/* init position (null, in his bag) */
                position = glm::ivec2(0, 0);

				/*  init pointers for rendering (in case where we render items in 3D in inventory) */
				mesh_ptr = meshLoad(mesh_path);
				shaders_ptr = shadersLoad(vShader_path, fShader_path);

				/*  Create Item */
				Item tmp_item = Item(position, id_inventory, value_inventory, (ItemType)type_inventory, durability_inventory, mesh_ptr, shaders_ptr);
				/*  Link to animations */
				tmp_item.animations_ptr =animationsLink;

				/*  add item on player (equiped) or in his bag (unequiped) */
				tmp_player.addItem(tmp_item);
                if(isEquiped) tmp_player.equip(tmp_item);
            }

			/*  Add player to map in assets */
            map.players.push_back(tmp_player);
        }

		/* ---- Items initialization ---- */

        file >> nb_Item;
        for (unsigned int i = 0; i < nb_Item; i++) {
			/* Item infos in file */
            file >> posX >> posY >> id >>  value >>  type >>  durability >>  mesh_path >> vShader_path >> fShader_path;

            getline(file, line);

			/* Init position and pointers for rendering */
            glm::ivec2 position = glm::ivec2(posX, posY);
            mesh_ptr = meshLoad(mesh_path);

			shaders_ptr = shadersLoad(vShader_path, fShader_path);

			/* Create item */
			Item tmp_item = Item(position, id, value, (ItemType)type, durability, mesh_ptr, shaders_ptr);
			/* Animations link */
			tmp_item.animations_ptr =animationsLink;

			/* Add item to map in assets */
			map.items.push_back(tmp_item);
        }

		/* ---- Enemies initialization ---- */
        file >> nb_Enemy;
        for (unsigned int i = 0; i < nb_Enemy; i++) {
            file >> posX >> posY >> scale >>  id >>  life >>  defense >>  power >>  detectRange >>  mesh_path >> vShader_path >> fShader_path;
            getline(file, line);
            glm::ivec2 position = glm::ivec2(posX, posY);
            mesh_ptr = meshLoad(mesh_path);
			shaders_ptr = shadersLoad(vShader_path, fShader_path);
			Enemy tmp_enemy = Enemy(position, glm::vec3(0,0,0), scale, id, life, defense, power, detectRange, mesh_ptr, shaders_ptr);
			tmp_enemy.animations_ptr =animationsLink;
			tmp_enemy.setMap(&map);
			map.characters.push_back(tmp_enemy);
        }

		/* ---- Traps initialization ---- */
        file >> nb_Traps;
        for (unsigned int i = 0; i < nb_Traps; i++) {
            file >> posX >> posY >> id >>  damages >>  timing >>  mesh_path >> vShader_path >> fShader_path;
            getline(file, line);
            glm::ivec2 position = glm::ivec2(posX, posY);
            mesh_ptr = meshLoad(mesh_path);
			shaders_ptr = shadersLoad(vShader_path, fShader_path);
			Trap tmp_trap = Trap(position, id, damages, timing, mesh_ptr, shaders_ptr);
			tmp_trap.animations_ptr =animationsLink;
            map.traps.push_back(tmp_trap);
        }

        file.close();
    } else
        cerr << "Cannot open " << pathFile << endl;
}

/* Open file of animations pack */
void Assets::loadAnimationsPack(EntityType type, glimac::FilePath animationsPackPath){
	ifstream file;
    file.open(animationsPackPath, ios::in);

	string line;
    if(file){
		getline(file, line);
		/*  Add animset ENCOUNTER */
		animations[type].push_back(Animation(application_path + ANIMATION_PATH + line));

		getline(file, line);
		/* add animset END */
		animations[type].push_back(Animation(application_path + ANIMATION_PATH + line));

		getline(file, line);
		/* add animset STAND */
		animations[type].push_back(Animation(application_path + ANIMATION_PATH + line));
    } else
        cerr << "Cannot open " << animationsPackPath << endl;
}

void Assets::reset(){
	meshes.clear();
	shaders.clear();
	map.clear();
}

void Assets::print(){
	std::cout << "*** ASSETS - BEGIN ***" << std::endl;
	std::cout << "Animations chargées : " << animations.size() << std::endl;
	std::cout << "Meshes chargés : " << meshes.size() << std::endl;
	std::cout << "Shaders chargés : " << shaders.size() << std::endl;
	std::cout << std::endl;
	std::cout << "*** Animations - BEGIN ***" << std::endl;
	for(std::map < EntityType, std::vector<Animation> >::iterator it = animations.begin();
		it!=animations.end();
		++it){
			std::cout << "Animation for : " << (*it).first << '\n';
			unsigned int i = 0;
			for(std::vector<Animation>::iterator it2 = (*it).second.begin();
				it2!=(*it).second.end();
				++it2){
					std::cout << "Set for : " << (EntityType)i << '\n';
					(*it2).print();
					std::cout << '\n';
					++i;
			}
	}
	std::cout << "*** Animations - END ***" << '\n';
	map.print();
	std::cout << std::endl;
	std::cout << "*** ASSETS - END ***" << '\n';
}
