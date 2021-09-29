#vertex
#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
    TexCoords = texCoords;
}
#fragment
#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D screen;

void main() {
    FragColor = texture(screen, TexCoords);
}