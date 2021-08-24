
#include <iostream>
#include "ModelManager.h"

/**
 * Creates a model from a file with a supported file format.
 * @param path File path + file.extension
 */
ModelManager::ModelManager(const char *path) : path(path), file(path, LITTLE_ENDIAN) {
    switch (file.getMagic()) {
        case FRES:
            break;
        default: std::cerr << "[BakeEngine]:[Error] Could not read file. File magic is not supported." << std::endl;
            break;
    }
}
