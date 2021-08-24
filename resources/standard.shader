#vertex
#version 330 core

// TODO: normals of vertices to normals of polygons, currently it is like a sphere

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normal;

out vec3 Color;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    TexCoord = texCoord;
    Color = color;
    vec4 modelPos = model * vec4(position, 1.0);
    gl_Position = projection * view * modelPos;
    FragPos = vec3(modelPos);
    Normal = mat3(transpose(inverse(model))) * normal;
}
#fragment
#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float roughness;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Color;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 outColor;

uniform sampler2D textureImage1;
uniform vec3 cameraPos;
uniform Material material;
#define NUM_POINT_LIGHTS 2
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform DirectionalLight dirLight;

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 camDir) {
    vec3 lightDirection = normalize(-light.direction);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * max(dot(normal, lightDirection), 0.0) * material.diffuse;
    vec3 specular = light.specular * pow(max(dot(camDir, reflectDirection), 0.0), material.roughness) * material.specular;
    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 camDir) {
    vec3 lightDirection = normalize(light.position - fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0/ (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * max(dot(normal, lightDirection), 0.0) * material.diffuse;
    vec3 specular = light.specular * pow(max(dot(camDir, reflectDirection), 0.0), material.roughness) * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 camDir) {
    vec3 lightDirection = normalize(light.position - fragPos);
    float theta = dot(lightDirection, normalize(-light.direction));

    if (theta < light.cutOff) {
        vec3 reflectDirection = reflect(-lightDirection, normal);

        vec3 ambient = light.ambient * material.ambient;
        vec3 diffuse = light.diffuse * max(dot(normal, lightDirection), 0.0) * material.diffuse;
        vec3 specular = light.specular * pow(max(dot(camDir, reflectDirection), 0.0), material.roughness) * material.specular;
    }
    return vec3(0.0);
}

void main() {

    vec3 norm = normalize(Normal);
    vec3 cameraDir = normalize(cameraPos - FragPos);

    vec3 lightResult = vec3(0.0);
    lightResult += calcDirectionalLight(dirLight, norm, cameraDir);
    for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
        lightResult += calcPointLight(pointLights[i], norm, FragPos, cameraDir);
    }

    outColor = texture(textureImage1, TexCoord) * vec4(Color * lightResult, 1.0);
}