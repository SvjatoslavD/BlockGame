#version 330 core
out vec4 FragColor;

in vec3 TexCoord;
flat in int Normal;
in float AO;

uniform sampler2DArray TextureArray1;

void main() {
    vec4 TexColor = texture(TextureArray1, TexCoord);
    float AmbientLighting = 1.0f - ((AO/3.f) * 0.8f);
    FragColor = vec4(TexColor.rgb * AmbientLighting, TexColor.a);
    // FragColor = vec4(AmbientLighting,AmbientLighting,AmbientLighting,1.0f);
}