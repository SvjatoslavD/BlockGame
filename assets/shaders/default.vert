#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCoord;
layout (location = 2) in int aNormal;
layout (location = 3) in float aAO;

out vec3 TexCoord;
flat out int Normal;
out float AO;

uniform vec2 atlasTileSize;
uniform mat4 cameraMatrix;
uniform mat4 modelMatrix;

void main() {
   gl_Position = cameraMatrix * modelMatrix * vec4(aPos/4.0f, 1.0f);
   // Opengl reads texture as upside if they are loaded normally
   TexCoord = vec3(aTexCoord.x,1.0f - aTexCoord.y,aTexCoord.z);
   Normal = aNormal;
   AO = aAO;
}