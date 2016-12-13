#include "mesh.h"

Mesh::Mesh() {
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
}

bool Mesh::loadFromFile(std::string file) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(file.c_str(), aiProcessPreset_TargetRealtime_Fast | aiProcess_Triangulate);
    
    if(!scene) {
        std::cout << "Error loading: " << file << " : " << importer.GetErrorString() << std::endl;
        return false;
    }
    
    // TODO Checker tout ça.
    // Faces are ONLY triangles
    
    aiMesh * mesh = scene->mMeshes[0];
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> indices;
    
    
    // vertices
    for(unsigned v = 0; v < mesh->mNumVertices; ++v) {
        vertices.push_back(glm::vec3(mesh->mVertices[v].x,
                                     mesh->mVertices[v].y,
                                     mesh->mVertices[v].z));
    }
    
    // faces (vertices indices)
    numFaces = mesh->mNumFaces;
    for(unsigned f = 0; f < mesh->mNumFaces; ++f) {
        indices.push_back(glm::ivec3(mesh->mFaces[f].mIndices[0],
                                     mesh->mFaces[f].mIndices[1],
                                     mesh->mFaces[f].mIndices[2]));
    }
    
    // send vertices to GPU
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // send indices to GPU
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::ivec3), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    // create VAO
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return true;
}

bool Mesh::loadShader(std::string vertexShader, std::string fragmentShader) {
    shader = glimac::loadProgram(vertexShader, fragmentShader);
    return true;
}

void Mesh::render() {
    shader.use();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3*numFaces, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}