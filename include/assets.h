#ifndef ASSETS_H
#define ASSETS_H

#include <vector>
#include <string>
#include <memory>
#include <map>
#include "FilePath.h"

#include "mesh.h"
#include "map.h"
#include "Program.h"
#include "animation.h"

class Assets {
public:
	const std::string SAVE_PATH = "res/saves/";
	const std::string DATA_PATH = "res/levels/";
	const std::string MESH_PATH = "res/meshes/";
	const std::string SHADER_PATH = "res/shaders/";
	const std::string ANIMATION_PATH = "res/animations/";

    glimac::FilePath application_path;

	std::map < EntityType, std::vector<Animation> > animations;
	std::vector<Mesh> meshes;
	std::vector<glimac::Program> shaders;
        
        /* temporaire (ou pas) */
        Mesh wall,
             ground,
             ceiling;
        /* shader de tous les éléments (quasiment) */
        std::shared_ptr<glimac::ProgramWrapper> generalShader;

	Map map;

	Assets(char* application_path);
	~Assets();
	void save(glimac::FilePath filePath);

	void load(std::string fileName, bool isNewGame);
	void loadEntities(glimac::FilePath fileName);
	std::shared_ptr<Mesh> meshLoad(glimac::FilePath mesh_path);
	std::shared_ptr<glimac::Program> shadersLoad(glimac::FilePath vShader_path, glimac::FilePath fShader_path);

	void loadAnimationsPack(EntityType type, glimac::FilePath animationsPackPath);

	void print();
};


#endif
