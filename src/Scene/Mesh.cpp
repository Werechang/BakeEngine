
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &elements, std::vector<GLTexture> &textures) :
            vertices(vertices), elements(elements), textures(textures),
            vbo(VertexBuffer((const float*)&vertices[0], vertices.size() * sizeof(Vertex))),
            ebo(ElementBuffer(&elements[0], elements.size()*sizeof(unsigned int))) {
    prepareMesh();
}

void Mesh::prepareMesh() {
    vao.bind();
    vbo.bind();
    ebo.bind();
    VertexAttributes attributes;
    attributes.addAttribute<float>(3);
    attributes.addAttribute<float>(3);
    attributes.addAttribute<float>(2);
    attributes.addAttribute<float>(3);
    vao.addVertexBuffer(vbo, attributes);
    vao.unbind();
}

void Mesh::render(GLShader& shader) const {
    for (unsigned int i = 0; i < textures.size(); i++) {
        GLTexture t = textures[i];
        t.bind(i);
        shader.uniform1i(t.getTypeName() + std::to_string(i+1), (int)i);
    }
}
