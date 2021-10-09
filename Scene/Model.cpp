
#include "Model.h"

#include <utility>

void Model::render(GLShader &shader) const {
    for (auto & mesh : meshes) {
        mesh.render(shader);
    }
}

Model::Model(std::string filePath) : path(std::move(filePath)){
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

void Model::load() const {
    switch (fileType) {
        case 1:
            fres->parse();
            break;
    }
}
