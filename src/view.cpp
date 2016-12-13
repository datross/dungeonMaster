#include <iostream>
#include "view.h"


View::View()
    : player_input(INPUT_NONE) {
    // Initialisation de la SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error while initializing SDL: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    // Version d'OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    // Double Buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    // taille de la fenêtre par défaut
    window_width = 1024;
    window_height = 680;
    
    // Création de la fenêtre
    window = SDL_CreateWindow("Dungeon Master", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if(window == 0)
    {
        std::cout << "Error while creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Création du contexte OpenGL
    context_gl = SDL_GL_CreateContext(window);
    if(context_gl == 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    // GLEW
    glewExperimental = GL_TRUE; 
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        exit(EXIT_FAILURE);
    }
}

View::~View() {
    SDL_GL_DeleteContext(context_gl);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void View::update() {
    player_input = INPUT_NONE;
    if(SDL_PollEvent(&event))
    {
        if(event.window.event == SDL_WINDOWEVENT_CLOSE)
            player_input = INPUT_QUIT;
    }
}

/*#include <assimp/Importer.hpp>
#include <GL/glut.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>*/
void View::render() {
    glClear(GL_COLOR_BUFFER_BIT);


	/*Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile("", aiProcessPreset_TargetRealtime_Fast);

	aiMesh *mesh = scene->mMeshes[0];

	float *vertexArray;
	float *normalArray;
	float *uvArray;

	int numVerts;

	numVerts = mesh->mNumFaces*3;

	vertexArray = new float[mesh->mNumFaces*3*3];
	normalArray = new float[mesh->mNumFaces*3*3];
	uvArray = new float[mesh->mNumFaces*3*2];

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace& face = mesh->mFaces[i];
		for(int j=0;j<3;j++) {
		aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
		memcpy(uvArray,&uv,sizeof(float)*2);
		uvArray+=2;

		aiVector3D normal = mesh->mNormals[face.mIndices[j]];
		memcpy(normalArray,&normal,sizeof(float)*3);
		normalArray+=3;

		aiVector3D pos = mesh->mVertices[face.mIndices[j]];
		memcpy(vertexArray,&pos,sizeof(float)*3);
		vertexArray+=3;
		}
	}

	uvArray-=mesh->mNumFaces*3*2;
	normalArray-=mesh->mNumFaces*3*3;
	vertexArray-=mesh->mNumFaces*3*3;*/


	/*glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	 
	glVertexPointer(3,GL_FLOAT,0,vertexArray);
	glNormalPointer(GL_FLOAT,0,normalArray);
	 
	glClientActiveTexture(GL_TEXTURE0_ARB);
	glTexCoordPointer(2,GL_FLOAT,0,uvArray);
	 
	glDrawArrays(GL_TRIANGLES,0,numVerts);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/


    SDL_GL_SwapWindow(window);
}

Player_input View::get_input() {
    return player_input;
}
