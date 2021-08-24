
#include "Model.h"

void Model::render(GLShader &shader) {
    for (Mesh m : meshes) {
        m.render(shader);
    }
}

Model::Model(const char *filePath) : path(filePath){

}

void Model::load() {

}
