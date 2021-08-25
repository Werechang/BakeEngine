//
// Created by tom on 25.08.2021.
//

#ifndef BAKEENGINE_FRES_H
#define BAKEENGINE_FRES_H


#include "../../File.h"

class Fres {
private:
    File file;
public:
    explicit Fres(File& file) : file(file) {};
    explicit Fres(const char* path) : file(File(path, BIG_ENDIAN)) {};
};


#endif //BAKEENGINE_FRES_H
