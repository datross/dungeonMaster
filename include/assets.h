#ifndef ASSETS_H
#define ASSETS_H

#include <vector>
#include <string>
#include <memory>
#include <map>

#include "mesh.h"
#include "map.h"
#include "Program.h"
#include "animation.h"

class Assets {
public:
	std::map < EntityType, std::vector<Animation> > animations;
	std::vector<Mesh> meshes;
	std::vector<glimac::Program> shaders;

	Map map;

	Assets();
	~Assets();
	void save(std::string fileName);

	void load(std::string fileName);
	void loadEntities(std::string fileName);
	std::shared_ptr<Mesh> meshLoad(std::string mesh_path);
	std::shared_ptr<glimac::Program> shadersLoad(std::string vShader_path, std::string fShader_path);

	void loadAnimationsPack(EntityType type, std::string animationsPackPath);
};


#endif
