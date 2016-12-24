#include "assets.h"

#include <fstream>
#include <iostream>

using namespace std;

Assets::Assets(){}
Assets::~Assets(){}


shared_ptr<Mesh> Assets::meshLoad(string mesh_path){
    for(vector<Mesh>::iterator it = meshes.begin(); it != meshes.end(); ++it){
        if((*it).path.compare(mesh_path) == 0){
            return shared_ptr<Mesh> (&(*it));
        }
    }

    Mesh * mesh = new Mesh();
    mesh->path = mesh_path;
    meshes.push_back(*mesh);
    return shared_ptr<Mesh>(mesh);
}
shared_ptr<glimac::Program> Assets::shadersLoad(string vShader_path, string fShader_path){
    for(vector<glimac::Program>::iterator it = shaders.begin(); it != shaders.end(); ++it){
        if((*it).vertexShaderPath.compare(vShader_path) == 0 &&
		   (*it).fragmentShaderPath.compare(fShader_path) == 0){
			   return shared_ptr<glimac::Program> (&(*it));
        }
    }

    glimac::Program * shader = new glimac::Program();
	(*shader) = glimac::loadProgram(vShader_path, fShader_path);
	shader->vertexShaderPath = vShader_path;
	shader->fragmentShaderPath = fShader_path;
    return shared_ptr<glimac::Program>(shader);
}

void Assets::load(string file_name){
    string levelsPath = "res/levels/";

    map.loadTerrain(levelsPath + file_name + ".ppm");
    loadEntities(levelsPath + file_name + ".txt");

    for(vector<Mesh>::iterator it = meshes.begin(); it != meshes.end(); ++it){
        (*it).loadFromFile((*it).path);
    }
}

void Assets::save(string pathFile){
    ofstream file;
    file.open(pathFile, ios::out | ios::trunc);

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
        cerr << "Cannot open or create " << pathFile << endl;

}

void Assets::loadEntities(string pathFile){

    ifstream file;
    file.open(pathFile, ios::in);

    unsigned int nb_Player, nb_Item, nb_Enemy, nb_Traps;
    string line;
    if(file){

        getline(file, line);
        map.name = line;

        int posX, posY, value, value_inventory, type, type_inventory, durability, durability_inventory;
        unsigned int life, defense, power, detectRange, damages, timing, nb_items_inventory, score, isEquiped;
        string id, id_inventory, mesh_path, vShader_path, fShader_path;
        float scale;
        shared_ptr<Mesh> mesh_ptr;
        shared_ptr<glimac::Program> shaders_ptr;

        file >> nb_Player;
        for (unsigned int i = 0; i < nb_Player; i++) {
            file >> posX >> posY >> scale >>  id >>  life >>  defense >>  power >> nb_items_inventory >> score >> mesh_path >> vShader_path >> fShader_path;
            getline(file, line);
            glm::ivec2 position = glm::ivec2(posX, posY);
            mesh_ptr = meshLoad(mesh_path);
			shaders_ptr = shadersLoad(vShader_path, fShader_path);
            Player tmp_player = Player(position, glm::vec3(0,0,0), scale, id, life, defense, power, score, mesh_ptr, shaders_ptr);

            for (unsigned int j = 0; j < nb_items_inventory; j++) {
                file >> id_inventory >>  value_inventory >> type_inventory >>  durability_inventory >> isEquiped >> mesh_path >> vShader_path >> fShader_path;
                getline(file, line);
                position = glm::ivec2(0, 0);
                if(isEquiped){
                    mesh_ptr = meshLoad(mesh_path);
					shaders_ptr = shadersLoad(vShader_path, fShader_path);
                    tmp_player.equip(Item(position, id_inventory, value_inventory, (ItemType)type_inventory, durability_inventory, mesh_ptr, shaders_ptr));
                }else{
                    mesh_ptr = meshLoad(mesh_path);
					shaders_ptr = shadersLoad(vShader_path, fShader_path);
                    tmp_player.addItem(Item(position, id_inventory, value_inventory, (ItemType)type_inventory, durability_inventory, mesh_ptr, shaders_ptr));
                }
            }
            map.players.push_back(tmp_player);
        }

        file >> nb_Item;
        for (unsigned int i = 0; i < nb_Item; i++) {
            file >> posX >> posY >> id >>  value >>  type >>  durability >>  mesh_path >> vShader_path >> fShader_path;
            getline(file, line);
            glm::ivec2 position = glm::ivec2(posX, posY);
            mesh_ptr = meshLoad(mesh_path);
			shaders_ptr = shadersLoad(vShader_path, fShader_path);
            map.items.push_back(Item(position, id, value, (ItemType)type, durability, mesh_ptr, shaders_ptr));
        }

        file >> nb_Enemy;
        for (unsigned int i = 0; i < nb_Enemy; i++) {
            file >> posX >> posY >> scale >>  id >>  life >>  defense >>  power >>  detectRange >>  mesh_path >> vShader_path >> fShader_path;
            getline(file, line);
            glm::ivec2 position = glm::ivec2(posX, posY);
            mesh_ptr = meshLoad(mesh_path);
			shaders_ptr = shadersLoad(vShader_path, fShader_path);
            map.characters.push_back(Enemy(position, glm::vec3(0,0,0), scale, id, life, defense, power, detectRange, mesh_ptr, shaders_ptr));
        }

        file >> nb_Traps;
        for (unsigned int i = 0; i < nb_Traps; i++) {
            file >> posX >> posY >> id >>  damages >>  timing >>  mesh_path >> vShader_path >> fShader_path;
            getline(file, line);
            glm::ivec2 position = glm::ivec2(posX, posY);
            mesh_ptr = meshLoad(mesh_path);
			shaders_ptr = shadersLoad(vShader_path, fShader_path);
            map.traps.push_back(Trap(position, id, damages, timing, mesh_ptr, shaders_ptr));
        }

        file.close();
    } else
        cerr << "Cannot open " << pathFile << endl;
}