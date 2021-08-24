
#ifndef BAKEENGINE_MODELMANAGER_H
#define BAKEENGINE_MODELMANAGER_H
#define FRES 1397051974

#include <string>
#include "File.h"

class ModelManager {
private:
    const char* path;
    File file;
public:
    explicit ModelManager(const char* path);
};


#endif
