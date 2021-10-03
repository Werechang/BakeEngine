
#ifndef BAKEENGINE_MESH_H
#define BAKEENGINE_MESH_H


#include <vector>
#include "Material.h"
#include "../Render/OpenGL/GLTexture.h"
#include "../Render/OpenGL/VertexArray.h"
#include "../Render/OpenGL/ElementBuffer.h"
#include "../Render/OpenGL/GLShader.h"

struct Vertex {
    float position[3];
    float color[3];
    float texCoords[2];
    float normal[3];
};
class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> elements;
    std::vector<GLTexture> textures;
    Material* material;

    VertexArray vao;
    VertexBuffer vbo;
    ElementBuffer ebo;

    void prepareMesh();
public:
    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& elements, std::vector<GLTexture>& textures);

    void render(GLShader& shader) const;
};


#endif //BAKEENGINE_MESH_H
