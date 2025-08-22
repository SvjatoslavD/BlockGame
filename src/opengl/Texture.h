//
// Created by svjat on 7/25/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include "Shader.h"

class Texture {
public:
    GLuint ID;
    GLenum type;
	Texture() = default;
    void Setup(const std::string& fileName, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, int tileSize);

    void TexUnit(Shader shader, const char* uniform, GLuint unit);
    void Bind();
    void Unbind();
    void Delete();
};

#endif // TEXTURE_H
