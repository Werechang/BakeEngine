
#ifndef BAKEENGINE_FILE_H
#define BAKEENGINE_FILE_H

#include "../Util/LogHelperBE.h"
#include "DataView.h"

class File : public DataView {
private:
    const char* path;
public:
    /**
     *
     * @param path file path
     * @param endian false = little, true = big
     */
    File(const char *path, bool endian);
    const char* getPath();
};


#endif
