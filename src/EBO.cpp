//
// Created by svjat on 7/24/2025.
//

#include "../header/EBO.h"

EBO::EBO(std::vector<unsigned int>& indices) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(float),indices.data(),GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ID);
}

void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

void EBO::Delete() {
	glDeleteBuffers(1,&ID);
}
