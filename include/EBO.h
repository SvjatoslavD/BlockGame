//
// Created by svjat on 7/24/2025.
//

#ifndef EBO_H
#define EBO_H

#include <GL/glew.h>

class EBO {
public:
	GLuint ID;
	EBO(GLuint* indicies, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif //EBO_H
