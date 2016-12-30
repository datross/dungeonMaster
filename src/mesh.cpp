#include "mesh.h"

Mesh::Mesh() {
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
}

Mesh::Mesh(const Mesh& mesh) {
	path	 = mesh.path;

    vbo      = mesh.vbo;
    ibo      = mesh.ibo;
    vao      = mesh.vao;
    numFaces = mesh.numFaces;

    uMVMatrix       = mesh.uMVMatrix;
    uMVPMatrix      = mesh.uMVPMatrix;
    uNormalMatrix   = mesh.uNormalMatrix;
    uTexture        = mesh.uTexture;
    uShininess      = mesh.uShininess;
    uLightDir_vs    = mesh.uLightDir_vs;
    uLightIntensity = mesh.uLightIntensity;
    uKs             = mesh.uKs;
    uKd             = mesh.uKd;
}

bool Mesh::loadFromFile(std::string file) {

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(file.c_str(), /*aiProcessPreset_TargetRealtime_Fast |*/ aiProcess_Triangulate/* | aiProcess_GenNormals*/);

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
        vertex.texCoord = glm::vec2(mesh->mTextureCoords[0]->x,
                                    mesh->mTextureCoords[0]->y);
        vertex.texCoord = glm::vec2(0, 0);
        vertices.push_back(vertex);
    }
std::cout << "hey" << std::endl;
    // faces (vertices indices)
    numFaces = mesh->mNumFaces;
    for(unsigned f = 0; f < mesh->mNumFaces; ++f) {
        indices.push_back(glm::ivec3(mesh->mFaces[f].mIndices[0],
                                     mesh->mFaces[f].mIndices[1],
                                     mesh->mFaces[f].mIndices[2]));
    }

    sendGeometryToGPU(vertices, indices);
    return true;
}

void Mesh::sendGeometryToGPU(std::vector<Vertex>& vertices, std::vector<glm::ivec3>& indices) {
    /* send vertices to GPU */
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* send indices to GPU */
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::ivec3), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* create VAO */
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
}

void Mesh::buildPlane(float w, float h) {
    std::vector<Vertex> vertices;
    std::vector<glm::ivec3> indices;
    for(unsigned x = 0; x < 2; ++x)
        for(unsigned y = 0; y < 2; ++y)
            vertices.push_back(Vertex(
                glm::vec3(x * w, y * h, 0),
                glm::vec3(0, 0, 1),
                glm::vec2(x, y)
            ));
    indices = {
        glm::ivec3(0, 1, 2),
        glm::ivec3(1, 3, 2) };
        
    sendGeometryToGPU(vertices, indices);
}

bool Mesh::setUniformsId(glimac::Program& shader) {

    uMVPMatrix = glGetUniformLocation(shader.getGLId(), "uMVPMatrix");
    uMVMatrix = glGetUniformLocation(shader.getGLId(), "uMVMatrix");
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

void Mesh::setKs(glm::vec3 value) {
    glUniform3fv(uKs, 1, glm::value_ptr(value));
}
void Mesh::setKd(glm::vec3 value) {
    glUniform3fv(uKd, 1, glm::value_ptr(value));
};
