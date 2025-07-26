#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D textureTop;
uniform sampler2D textureBottom;
uniform sampler2D textureSides;
uniform int drawnSide;

void main()
{
    switch (drawnSide) {
        case 0:
        FragColor = texture(textureTop, TexCoord);
        break;
        case 1:
        FragColor = texture(textureBottom, TexCoord);
        break;
        default:
        FragColor = texture(textureSides, TexCoord);
        break;
    }
}