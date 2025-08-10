//
// Created by svjat on 7/25/2025.
//

#include "Texture.h"

Texture::Texture(const std::string& fileName, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
    // Assigns the type of the texture ot the texture object
    type = texType;

    // Generates an OpenGL texture object
    glGenTextures(1, &ID);
    // Assigns the texture to a Texture Unit
    glActiveTexture(slot);
    glBindTexture(texType, ID);

    // Configures the type of algorithm that is used to make the image smaller or bigger
    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Disable mipmaps for the time being due to mipmaps created based off whole tilemap
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Configures the way the texture repeats (if it does at all)
    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    int tile_size = 16;

    sf::Image tilemap;
    bool image_loaded = tilemap.loadFromFile(fileName);


    if (image_loaded) {
        const std::uint8_t* data = tilemap.getPixelsPtr();

        // Assigns the image to the OpenGL Texture object
        glTexImage2D(texType, 0, GL_RGBA, tile_size * 5, tile_size * 5, 0, format, pixelType, data);

        // Generates MipMaps
        glGenerateMipmap(texType);

        // Unbinds the OpenGL Texture object so that it can't accidentally be modified
        glBindTexture(texType, 0);
    }
};

void Texture::TexUnit(Shader shader, const char* uniform, GLuint unit) {
    // Gets the location of the uniform
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    // Shader needs to be activated before changing the value of a uniform
    shader.Activate();
    // Sets the value of the uniform
    glUniform1i(texUni, unit);
}

void Texture::Bind() { glBindTexture(type, ID); }

void Texture::Unbind() { glBindTexture(type, 0); }

void Texture::Delete() { glDeleteTextures(1, &ID); }