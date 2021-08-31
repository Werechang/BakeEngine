
#include <fstream>
#include "File.h"

// TODO: better exception handling
/**
 * Creates a vector (array) to buffer the file data. Helper methods to parse the data easier.
 * @param path File path (includes the file itself + the extension)
 * @param endian LITTLE_ENDIAN (0) or BIG_ENDIAN (1)
 */
File::File(const char *path, bool endian) : DataView(endian), path(path) {
    LogHelperBE::pushName("File");
    this->setEndian(endian);

    std::ifstream input(path, std::ios::binary);
    if (!input) {
        LogHelperBE::error("File does not exist!");
        throw std::exception("File does not exist!");
    } else {
        bytes = std::vector<unsigned char>(std::istreambuf_iterator<char>(input), {});
    }
    LogHelperBE::popName();
}

const char *File::getPath() {
    return path;
}