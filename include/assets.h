#ifndef ASSETS_H
#define ASSETS_H

#include <vector>
#include <string>
#include <memory>

#include "mesh.h"
#include "map.h"

class Assets {
public:
	std::vector<Mesh> meshes;
	Map map;

	Assets();
	~Assets();
	void load(std::string fileName);
	void save(std::string fileName);
	void loadEntities(std::string fileName);
	std::shared_ptr<Mesh> meshLoad(std::string mesh_path);
};


#endif
