
#ifndef BAKEENGINE_FILE_H
#define BAKEENGINE_FILE_H

#include "../Util/LogHelperBE.h"
#include "DataView.h"

class File : public DataView {
private:
    std::string path;
public:
    /**
     *
     * @param path file path
     * @param endian false = little, true = big
     */
    File(std::string& path, bool endian);
    std::string& getPath();
};


#endif
