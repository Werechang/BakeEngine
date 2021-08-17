#vertex
#version 150 core

// TODO: normals of vertices to normals of polygons, currently it is like a sphere

in vec3 position;
in vec3 color;
in vec2 texCoord;
in vec3 normal;

out vec3 Color;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 vp;
uniform mat4 model;

void main() {
    TexCoord = texCoord;
    Color = color;
    vec4 modelPos = model * vec4(position, 1.0);
    gl_Position = vp * modelPos;
    FragPos = vec3(modelPos);
    Normal = normal;
}
#fragment
#version 150 core

in vec3 Color;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 outColor;

uniform sampler2D tex;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main() {

    float specularStrength = 1.0;
    float ambientStrength = 0.1;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 cameraDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 specular = specularStrength * pow(max(dot(cameraDir, reflectDir), 0.0), 256) * lightColor;

    vec3 diffuse = max(dot(norm, lightDir), 0.0) * lightColor;

    vec3 ambientLight = ambientStrength * lightColor;

    vec3 lightResult = ambientLight + diffuse + specular;
    outColor = texture(tex, TexCoord) * vec4(Color * lightResult, 1.0);
}