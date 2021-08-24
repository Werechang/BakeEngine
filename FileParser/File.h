
#ifndef BAKEENGINE_FILE_H
#define BAKEENGINE_FILE_H
#define LITTLE_ENDIAN 0
#define BIG_ENDIAN 1

#include <vector>

class File {
private:
    const char* path;
    std::vector<char> bytes;
    unsigned int index = 0;
    bool endian;
public:
    /**
     *
     * @param path file path
     * @param endian false = little, true = big
     */
    File(const char* path, bool endian);
    int getMagic();
    int readInt();
    int readShort();
    int readByte();
    void seek(int offset);
    void setEndian(bool isBigEndian);
};


#endif
