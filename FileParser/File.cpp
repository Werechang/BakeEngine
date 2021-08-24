
#include <fstream>
#include <string>
#include "File.h"

/**
 * Creates a vector (array) to buffer the file data. Helper methods to parse the data easier.
 * @param path File path (includes the file itself + the extension)
 * @param endian LITTLE_ENDIAN (0) or BIG_ENDIAN (1)
 */
File::File(const char *path, bool endian) : path(path), endian(endian) {
    std::ifstream input(path, std::ios::binary);
    bytes = std::vector<char>((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
}

/**
 *
 * @return an int from the first four bytes of the file. little endian
 */
int File::getMagic() {
    index = 4;
    return bytes[0] + (bytes[1] << 8) + (bytes[2] << 16) + (bytes[3] << 24);
}

int File::readInt() {
    index += 4;
    if (endian) {
        return (bytes[index-4] << 24) + (bytes[index-3] << 16) + (bytes[index-2] << 8) + bytes[index-1];
    } else {
        return bytes[index-4] + (bytes[index-3] << 8) + (bytes[index-2] << 16) + (bytes[index-1] << 24);
    }
}

int File::readShort() {
    index += 2;
    if (endian) {
        return (bytes[index-2] << 8) + bytes[index-1];
    } else {
        return bytes[index-2] + (bytes[index-1] << 8);
    }
}

int File::readByte() {
    ++index;
    return (int)bytes[index-1];
}

/**
 * Sets the index of the array to offset
 * @param offset
 */
void File::seek(int offset) {
    this->index = offset;
}

void File::setEndian(bool isBigEndian) {
    this->endian = isBigEndian;
}
