
#ifndef BAKEENGINE_DATAVIEW_H
#define BAKEENGINE_DATAVIEW_H
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
    unsigned int index = 0;
    bool endian;

    DataView(std::vector<unsigned char>& data, bool endian);
    explicit DataView(bool endian);
    unsigned int readUInt();
    int readInt();
    int readShort();
    int readByte();
    std::string readString(unsigned int length);
    float readFloat();
    int readInt(unsigned int offset);
    unsigned int readUInt(unsigned int offset);
    int readByte(unsigned int offset);
    std::string readString(unsigned int offset, unsigned int length, bool nullTerminated);
    float readFloat(unsigned int offset);
    void seek(int offset);
    void setEndian(bool isBigEndian);
    DataView getBufferSlice(unsigned int offset, unsigned int length);
    std::vector<unsigned char> getData();

    int readShort(unsigned int offset);
};


#endif
