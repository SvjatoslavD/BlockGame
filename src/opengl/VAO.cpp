//
// Created by svjat on 7/24/2025.
//

#include "VAO.h"

VAO::VAO() {}

void VAO::GenerateID() {
	glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
    Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
}

void VAO::Bind() { glBindVertexArray(ID); }

void VAO::Unbind() { glBindVertexArray(0); }

void VAO::Delete() { glDeleteVertexArrays(1, &ID); }
