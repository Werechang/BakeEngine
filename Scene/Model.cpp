
#include "Model.h"

void Model::render(GLShader &shader) {
    for (Mesh m : meshes) {
        m.render(shader);
    }
}

Model::Model(const char *filePath) : path(filePath){
    LogHelperBE::pushName("Model");
    fileType = -1;
    try {
        File file(path, BIG_ENDIAN);
        switch (file.readInt()) {
            case FRES: {
                if (file.readString(4) != "    ") {
                    LogHelperBE::error("File may be corrupted or incomplete.");
                    break;
                }
                fres = new Fres(file);
                fileType = 1;
                break;
            }
            default: LogHelperBE::error("File type is not supported or corrupted.");
            break;
        }
    } catch (std::exception& e) {};
    LogHelperBE::popName();
}

void Model::load() {
    switch (fileType) {
        case 1:
            fres->parse();
            break;
    }
}
