#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include "glm.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Program.h"

enum VertexAttribute {
    VERTEX_ATTR_POSITION = 0,
    VERTEX_ATTR_COLOR = 1
};

class Mesh {
public:
    Mesh();
    ~Mesh();
    
    bool loadFromFile(std::string file);
    bool loadShader(std::string vertexShader, std::string fragmentShader);
    void render();
private:
    GLuint vbo, ibo, vao;
    unsigned numFaces;
    glimac::Program shader;
};

#endif