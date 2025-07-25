//
// Created by svjat on 7/24/2025.
//

#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>

class VBO {
public:
	GLuint ID;
	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};
#endif //VBO_H
