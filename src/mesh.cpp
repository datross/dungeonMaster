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
    const aiScene *scene = importer.ReadFile(file.c_str(), /*aiProcessPreset_TargetRealtime_Fast |*/ aiProcess_Triangulate | aiProcess_GenSmoothNormals);
    
    if(!scene) {
        std::cout << "Error loading: " << file << " : " << importer.GetErrorString() << std::endl;
        return false;
    }
    
    // TODO Checker tout ça.
    // Faces are ONLY triangles
    
    aiMesh * mesh = scene->mMeshes[0];
    
    std::vector<Vertex> vertices;
    std::vector<glm::ivec3> indices;
    
    
    // vertices
    for(unsigned v = 0; v < mesh->mNumVertices; ++v) {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[v].x,
                                    mesh->mVertices[v].y,
                                    mesh->mVertices[v].z);
        vertex.normal = glm::vec3(mesh->mNormals[v].x,
                                  mesh->mNormals[v].y,
                                  mesh->mNormals[v].z);
//         vertex.texCoord = glm::vec2(mesh->mTextureCoords[0]->x,
//                                     mesh->mTextureCoords[0]->y);
        vertex.texCoord = glm::vec2(0, 0);
        vertices.push_back(vertex);
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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
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
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORD);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return true;
}

bool Mesh::loadShader(std::string vertexShader, std::string fragmentShader) {
    shader = glimac::loadProgram(vertexShader, fragmentShader);
    
    uMVPMatrix = glGetUniformLocation(shader.getGLId(), "uMVPMatrix");
    uNormalMatrix = glGetUniformLocation(shader.getGLId(), "uNormalMatrix");
    uTexture = glGetUniformLocation(shader.getGLId(), "uTexture");
    uShininess = glGetUniformLocation(shader.getGLId(), "uShininess");
    uLightDir_vs = glGetUniformLocation(shader.getGLId(), "uLightDir_vs");
    uLightIntensity = glGetUniformLocation(shader.getGLId(), "uLightIntensity");
    uKs = glGetUniformLocation(shader.getGLId(), "uKs");
    uKd = glGetUniformLocation(shader.getGLId(), "uKd");
    
    return true;
}

void Mesh::render() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3*numFaces, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::activateShader() {
    shader.use();
}

void Mesh::setMVMatrix(glm::mat4 mat) {
    glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(mat));
}

void Mesh::setMVPMatrix(glm::mat4 mat) 
{
    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mat));
}

void Mesh::setNormalMatrix(glm::mat4 mat) {
    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(mat));
}

void Mesh::setLightDir_vs(glm::vec3 dir) {
    glUniform3fv(uLightDir_vs, 1, glm::value_ptr(dir));
}

void Mesh::setLightIntensity(glm::vec3 intensity) {
    glUniform3fv(uLightIntensity, 1, glm::value_ptr(intensity));
}

void Mesh::setShininess(float value) {
    glUniform1f(uShininess, value);
}

void Mesh::setKs(float value) {
    glUniform1f(uKs, value);
}
void Mesh::setKd(float value) {
    glUniform1f(uKd, value);
};