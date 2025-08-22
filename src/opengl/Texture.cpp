//
// Created by svjat on 7/25/2025.
//

#include "Texture.h"

void Texture::Setup(const std::string& fileName, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, int tileSize) {
    // Assigns the type of the texture ot the texture object
    type = texType;

    // Generates an OpenGL texture object
    glGenTextures(1, &ID);
    // Assigns the texture to a Texture Unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(texType, ID);

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    sf::Image image;
    sf::Image tile(sf::Vector2u(tileSize,tileSize));
    const bool image_loaded = image.loadFromFile(fileName);

    const int image_width = image.getSize().x;
    const int image_height = image.getSize().y;

    if (image_loaded) {
        if (texType == GL_TEXTURE_2D) {
            const std::uint8_t* data = image.getPixelsPtr();

            glTexImage2D(texType, 0, GL_RGBA, image_width, image_height, 0, format, pixelType, data);
            glGenerateMipmap(texType);

            // Unbinds the OpenGL Texture object so that it can't accidentally be modified
            glBindTexture(texType, 0);
        }
        else if (texType == GL_TEXTURE_2D_ARRAY) {
            int mip_level_count = 2;
            int rows = image_width/ tileSize;
            int cols = image_height/ tileSize;

            glTexStorage3D(texType, mip_level_count,GL_RGBA8, tileSize,tileSize, rows*cols);

            int tile_amount = image_width/tileSize * image_height/tileSize;
            for (int i = 0; i < tile_amount; i++) {
                bool copied = tile.copy(image,sf::Vector2u(0,0),sf::IntRect(sf::Vector2i((i%cols) * tileSize,(i/rows) * tileSize),sf::Vector2i(tileSize,tileSize)));

                // std::string msg = copied ? "Succeeded" : "Failed";
                // std::cout << msg << std::endl;

                const std::uint8_t* data = tile.getPixelsPtr();
                glTexSubImage3D(texType, 0, 0, 0, i, tileSize,tileSize, 1,GL_RGBA, GL_UNSIGNED_BYTE, data);
            }

            glGenerateMipmap(texType);
        }
        else {
            std::cerr << "Texture type not supported!" << std::endl;
        }
    }
    else {
        std::cerr << "Failed to load image from file: " << fileName << std::endl;
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