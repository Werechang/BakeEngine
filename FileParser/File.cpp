
#include <fstream>
#include "File.h"

/**
 * Creates a vector (array) to buffer the file data. Helper methods to parse the data easier.
 * @param path File path (includes the file itself + the extension)
 * @param endian LITTLE_ENDIAN (0) or BIG_ENDIAN (1)
 */
File::File(std::string& path, bool endian) : DataView(endian), path(path) {
    LogHelperBE::pushName("File");

    std::ifstream input(path, std::ios::binary);
    if (!input) {
        LogHelperBE::error("File does not exist!");
        throw std::runtime_error("File does not exist!");
    } else {
        bytes = std::vector<unsigned char>(std::istreambuf_iterator<char>(input), {});
    }
    LogHelperBE::popName();
}

std::string& File::getPath() {
    return path;
}