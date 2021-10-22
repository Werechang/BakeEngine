
#include <fstream>
#include "File.h"
#include "../../Application/Application.h"

/**
 * Creates a vector (array) to buffer the file data. Helper methods to parse the data easier.
 * @param path File path (includes the file itself + the extension)
 * @param endian LITTLE_ENDIAN (0) or BIG_ENDIAN (1)
 */
File::File(const std::string& path, bool endian) : DataView(endian), path(Application::absolutePath + path) {
    LogHelperBE::pushName("File");

    std::ifstream elseStream(this->path, std::ios::binary);
    if (!elseStream) {
        LogHelperBE::error("File does not exist!");
        throw std::runtime_error("File does not exist!");
    } else {
        bytes = std::vector<unsigned char>(std::istreambuf_iterator<char>(elseStream), {});
    }
    LogHelperBE::popName();
}