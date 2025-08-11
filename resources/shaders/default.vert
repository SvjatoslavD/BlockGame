#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCoord;

out vec3 TexCoord;

uniform vec2 atlasTileSize;
uniform mat4 cameraMatrix;
uniform mat4 modelMatrix;

void main() {
   gl_Position = cameraMatrix * modelMatrix * vec4(aPos, 1.0f);
   // Opengl reads texture as upside if they are loaded normally
   TexCoord = vec3(aTexCoord.x,1.0f - aTexCoord.y,aTexCoord.z);
}