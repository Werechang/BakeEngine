
#ifndef BAKEENGINE_MODEL_H
#define BAKEENGINE_MODEL_H


#include "../Render/OpenGL/GLShader.h"
#include "Mesh.h"
#include "../FileParser/Model/fres/Fres.h"
#include "../Util/LogHelperBE.h"

#define FRES 1179796819

class Model {
private:
    std::vector<Mesh> meshes;
    const char* path;
    int fileType;
    Fres* fres;
public:
    explicit Model(const char* filePath);
    void load();
    void render(GLShader& shader);
};


#endif
