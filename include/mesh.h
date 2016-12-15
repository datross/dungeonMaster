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
    VERTEX_ATTR_NORMAL = 1,
    VERTEX_ATTR_TEXCOORD = 2
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    
    Vertex(){}
    Vertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _texCoord)
        : position(_position), normal(_normal), texCoord(_texCoord) {}
};

class Mesh {
public:
    Mesh();
    ~Mesh();
    
    bool loadFromFile(std::string file);
    bool loadShader(std::string vertexShader, std::string fragmentShader);
    void render();
    
    void activateShader();
    
    void setMVMatrix(glm::mat4);
    void setMVPMatrix(glm::mat4);
    void setNormalMatrix(glm::mat4);
    void setShininess(float);
    void setLightDir_vs(glm::vec3);
    void setLightIntensity(glm::vec3);
    void setKs(glm::vec3);
    void setKd(glm::vec3);
private:
    GLuint vbo, ibo, vao;
    unsigned numFaces;
    glimac::Program shader;
    
    // uniform
    GLint uMVMatrix;
    GLint uMVPMatrix;
    GLint uNormalMatrix;
    GLint uTexture;
    GLint uShininess;
    GLint uLightDir_vs;
    GLint uLightIntensity;
    GLint uKs;
    GLint uKd;
};

#endif