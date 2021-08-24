
#ifndef BAKEENGINE_MODEL_H
#define BAKEENGINE_MODEL_H


#include "../Render/OpenGL/GLShader.h"
#include "Mesh.h"

class Model {
private:
    std::vector<Mesh> meshes;
    const char* path;

    void load();
public:
    Model(const char* filePath);

    void render(GLShader& shader);
};


#endif
