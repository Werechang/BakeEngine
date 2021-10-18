
#pragma once
#define LITTLE_ENDIAN 0
#define BIG_ENDIAN 1

#include <string>
#include <vector>

union bytes2Float {
    unsigned char bytes[4];
    float f;
};
class DataView {
protected:
    std::vector<unsigned char> bytes;
public:
    uint32_t index = 0;
    bool endian;

    DataView(std::vector<unsigned char>& data, bool endian);
    explicit DataView(bool endian);
    uint32_t readUInt();
    int readInt();
    int readShort();
    int readByte();
    std::string readString(uint32_t length);
    float readFloat();
    double readFixed();
    uint32_t readDate();
    int readInt(uint32_t offset);
    uint32_t readUInt(uint32_t offset);
    int readByte(uint32_t offset);
    std::string readString(uint32_t offset, uint32_t length, bool nullTerminated);
    float readFloat(uint32_t offset);
    void seek(uint32_t offset);
    void skip(uint32_t offset);
    uint32_t getOffset();
    void setEndian(bool isBigEndian);
    DataView getBufferSlice(uint32_t offset, uint32_t length);
    std::vector<unsigned char> getData();

    int readShort(uint32_t offset);
};
