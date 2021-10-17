
#pragma once

#include "../Util/LogHelperBE.h"
#include "DataView.h"

class File : public DataView {
private:
    const std::string path;
    std::vector<std::string> lines;
public:
    /**
     *
     * @param path file path
     * @param endian false = little, true = big
     */
    File(const std::string& path, bool endian);
};
