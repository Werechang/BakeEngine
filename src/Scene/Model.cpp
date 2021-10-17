
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
                fres = std::make_unique<Fres>(file);
                fileType = 1;
                break;
            }
            case COLLADA: {
                if (file.readString(0x0, 38, false) != R"(<?xml version="1.0" encoding="utf-8"?>)") {
                    LogHelperBE::error("File may be corrupted or incomplete.");
                    break;
                }
                collada = std::make_unique<Collada>(path);
                fileType = 2;
                break;
            }
            default: LogHelperBE::error("File type is not supported or corrupted.");
            break;
        }
    } catch (std::exception&) {};
    LogHelperBE::popName();
}

void Model::load() const {
    switch (fileType) {
        case 1: {
            fres->parse();
            break;
        }
        case 2: {
            collada->parse();
            break;
        }
    }
}
