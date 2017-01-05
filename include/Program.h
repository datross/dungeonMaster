#ifndef PROGRAM_H
#define PROGRAM_H

#include <GL/glew.h>
#include "Shader.h"
#include "FilePath.h"
#include <string>

namespace glimac {

class Program {
private:

	GLuint m_nGLId;
public:
	std::string vertexShaderPath;
	std::string fragmentShaderPath;

	Program(): m_nGLId(glCreateProgram()) {
	}
	~Program() {
		glDeleteProgram(m_nGLId);
	}
	Program(const Program& p){
		m_nGLId = p.m_nGLId;
		vertexShaderPath = p.vertexShaderPath;
		fragmentShaderPath = p.fragmentShaderPath;
	}
	
	GLuint getGLId() const {
		return m_nGLId;
	}
	void attachShader(const Shader& shader) {
		glAttachShader(m_nGLId, shader.getGLId());
	}
	void use() const {
		glUseProgram(m_nGLId);
	}

	bool link();
	const std::string getInfoLog() const;

};

// Build a GLSL program from source code
//Program buildProgram(const GLchar* vsSrc, const GLchar* fsSrc);

// Load source code from files and build a GLSL program
Program loadProgram(const FilePath& vsFile, const FilePath& fsFile);

/* wrapper pour Program */
struct ProgramWrapper {
    Program program;
    ProgramWrapper(std::string vs, std::string fs) 
        : program(glimac::loadProgram("res/shaders/3D.vs.glsl",
                    "res/shaders/pointlight.fs.glsl"))
        {}
};

}

#endif
