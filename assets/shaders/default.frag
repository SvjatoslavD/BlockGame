#version 330 core
out vec4 FragColor;

in vec3 TexCoord;
in vec3 Normal;

uniform sampler2DArray TextureArray1;

void main() {
    FragColor = texture(TextureArray1, TexCoord);
}