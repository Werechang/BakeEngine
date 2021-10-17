
#pragma once

#include "../Render/OpenGL/GLShader.h"
#include "Mesh.h"
#include "../FileParser/Model/fres/Fres.h"
#include "../Util/LogHelperBE.h"
#include "../FileParser/Model/Collada.h"

#define FRES 1179796819
#define COLLADA 1010792557

class Model {
private:
    std::vector<Mesh> meshes;
    std::string path;
    int fileType;
    std::unique_ptr<Fres> fres;
    std::unique_ptr<Collada> collada;
public:
    explicit Model(std::string filePath);

    void load() const;
    void render(GLShader& shader) const;
};
