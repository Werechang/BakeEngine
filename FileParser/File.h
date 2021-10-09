
#ifndef BAKEENGINE_FILE_H
#define BAKEENGINE_FILE_H

#include "../Util/LogHelperBE.h"
#include "DataView.h"

class File : public DataView {
private:
    const std::string path;
public:
    /**
     *
     * @param path file path
     * @param endian false = little, true = big
     */
    File(const std::string& path, bool endian);
};


#endif
