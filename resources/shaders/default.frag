#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D textureTop;

void main() {
    FragColor = texture(textureTop, TexCoord);
}