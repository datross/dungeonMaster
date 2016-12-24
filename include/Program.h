#ifndef PROGRAM_H
#define PROGRAM_H

#include <GL/glew.h>
#include "Shader.h"
#include "FilePath.h"
#include <string>

namespace glimac {

class Program {
public:
	Program(): m_nGLId(glCreateProgram()) {
	}

	~Program() {
		glDeleteProgram(m_nGLId);
	}

	Program(Program&& rvalue): m_nGLId(rvalue.m_nGLId) {
		rvalue.m_nGLId = 0;
	}

// 	Program& operator =(Program&& rvalue) {
// 		m_nGLId = rvalue.m_nGLId;
// 		rvalue.m_nGLId = 0;
// 		return *this;
// 	}

        Program& operator=(const Program& source) {
            m_nGLId = source.m_nGLId;
            return *this;
        }

	GLuint getGLId() const {
		return m_nGLId;
	}

	void attachShader(const Shader& shader) {
		glAttachShader(m_nGLId, shader.getGLId());
	}

	bool link();

	const std::string getInfoLog() const;

	void use() const {
		glUseProgram(m_nGLId);
	}

	std::string vertexShaderPath;
	std::string fragmentShaderPath;



private:
	Program(const Program&);
// 	Program& operator =(const Program&);

	GLuint m_nGLId;
};

// Build a GLSL program from source code
Program buildProgram(const GLchar* vsSrc, const GLchar* fsSrc);

// Load source code from files and build a GLSL program
Program loadProgram(const FilePath& vsFile, const FilePath& fsFile);


}

#endif
