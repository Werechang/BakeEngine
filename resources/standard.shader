#vertex
#version 150 core

in vec3 position;
in vec3 color;
in vec2 texCoord;

out vec3 Color;
out vec2 TexCoord;

uniform mat4 transform;

void main() {
    TexCoord = texCoord;
    Color = color;
    gl_Position = transform * vec4(position, 1.0);
}
#fragment
#version 150 core

in vec3 Color;
in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D tex;

void main() {
    outColor = texture(tex, TexCoord) * vec4(Color, 1.0);
}