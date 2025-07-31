//
// Created by svjat on 7/25/2025.
//

#include"../header/Texture.h"

Texture::Texture(const std::string& fileName, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, int atlasX, int atlasY)
{
	// Assigns the type of the texture ot the texture object
	type = texType;

	// Generates an OpenGL texture object
	glGenTextures(1, &ID);
	// Assigns the texture to a Texture Unit
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	// Configures the type of algorithm that is used to make the image smaller or bigger
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configures the way the texture repeats (if it does at all)
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	int tile_size = 16;

	sf::Image tilemap;
	bool image_loaded = tilemap.loadFromFile(fileName);

	sf::Image tile(sf::Vector2u(tile_size,tile_size),sf::Color::Blue);

	// There would need to be an external map with block names and Pixel arrays to store more blocks
	// Maybe this would be a tilemap class


	bool tile_copied = tile.copy(tilemap,sf::Vector2u(0,0),sf::Rect(sf::Vector2((atlasX)*tile_size,(atlasY)*tile_size),sf::Vector2(tile_size,tile_size)),false);
	if (tile_copied) {
		const std::uint8_t *data = tile.getPixelsPtr();

		// Assigns the image to the OpenGL Texture object
		glTexImage2D(texType, 0, GL_RGBA, tile_size,tile_size, 0, format, pixelType, data);

		// Generates MipMaps
		glGenerateMipmap(texType);

		// Unbinds the OpenGL Texture object so that it can't accidentally be modified
		glBindTexture(texType, 0);
	}

};

void Texture::texUnit(Shader shader, const char* uniform, GLuint unit) {
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.use();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}

void Texture::Bind() {
	glBindTexture(type, ID);
}

void Texture::Unbind() {
	glBindTexture(type, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}