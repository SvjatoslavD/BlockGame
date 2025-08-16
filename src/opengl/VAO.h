//
// Created by svjat on 7/24/2025.
//

#ifndef VAO_H
#define VAO_H

#include <GL/glew.h>

class VAO {
public:
    GLuint ID;
    VAO();

    void LinkAttrib(GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
    void Bind();
    void Unbind();
    void Delete();
};
#endif // VAO_H
