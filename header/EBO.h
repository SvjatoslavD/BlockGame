//
// Created by svjat on 7/24/2025.
//

#ifndef EBO_H
#define EBO_H

#include <vector>
#include <GL/glew.h>

class EBO {
public:
	GLuint ID;
	EBO();
	EBO(std::vector<unsigned int>& indices);

	void SetData(std::vector <unsigned int>& indices);
	void Bind();
	void Unbind();
	void Delete();
};

#endif //EBO_H
