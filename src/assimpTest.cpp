/* ----------------------------------------------------------------------------
// Simple sample to prove that Assimp is easy to use with OpenGL.
// It takes a file name as command line parameter, loads it using standard
// settings and displays it.
//
// If you intend to _use_ this code sample in your app, do yourself a favour
// and replace immediate mode calls with VBOs ...
//
// The vc8 solution links against assimp-release-dll_win32 - be sure to
// have this configuration built.
// ----------------------------------------------------------------------------
*/

/*#include <stdlib.h>
#include <stdio.h>

/* assimp include files. These three are usually needed. 
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/ai_assert.h>
#include <assimp/mesh.h>
#include <assimp/vector3.h>
#include <assimp/Importer.hpp>

#include <GL/glut.h>


int main(int argc, char **argv)
{
	Assimp::Importer importer;
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
	vertexArray-=mesh->mNumFaces*3*3;


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	 
	glVertexPointer(3,GL_FLOAT,0,vertexArray);
	glNormalPointer(GL_FLOAT,0,normalArray);
	 
	glClientActiveTexture(GL_TEXTURE0_ARB);
	glTexCoordPointer(2,GL_FLOAT,0,uvArray);
	 
	glDrawArrays(GL_TRIANGLES,0,numVerts);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	return 0;
}*/
