
#include "DataView.h"

DataView::DataView(std::vector<unsigned char> &data, bool endian) : bytes(data), endian(endian){
}

DataView::DataView(bool endian) : bytes(std::vector<unsigned char>()), endian(endian) {
}

unsigned int DataView::readUInt() {
    index += 4;
    if (endian) {
        return ((bytes[index-4] & 0xff) << 24) | ((bytes[index-3] & 0xff) << 16) | ((bytes[index-2] & 0xff) << 8) | (bytes[index-1] & 0xff);
    } else {
        return (bytes[index-4] & 0xff) | ((bytes[index-3] & 0xff) << 8) | ((bytes[index-2] & 0xff) << 16) | ((bytes[index-1] & 0xff) << 24);
    }
}

int DataView::readInt() {
    index += 4;
    if (endian) {
        return ((bytes[index-4] & 0xff) << 24) | ((bytes[index-3] & 0xff) << 16) | ((bytes[index-2] & 0xff) << 8) | (bytes[index-1] & 0xff);
    } else {
        return (bytes[index-4] & 0xff) | ((bytes[index-3] & 0xff) << 8) | ((bytes[index-2] & 0xff) << 16) | ((bytes[index-1] & 0xff) << 24);
    }
}

int DataView::readShort() {
    index +=2;
    if (endian) {
        return ((bytes[index-2] & 0xff) << 8) | (bytes[index-1] & 0xff);
    } else {
        return (bytes[index-2] & 0xff) | ((bytes[index-1] & 0xff) << 8);
    }
}

int DataView::readByte() {
    return (int)(bytes[index++] & 0xff);
}

std::string DataView::readString(unsigned int length) {
    std::string buffer;
    for (auto i = 0; i < length; i++) {
        buffer += (char)(bytes[index] & 0xff);
        index++;
    }
    return buffer;
}

float DataView::readFloat() {
    bytes2Float bf{};
    index += 4;
    if (endian) {
        bf.bytes[3] = bytes[index-4];
        bf.bytes[2] = bytes[index-3];
        bf.bytes[1] = bytes[index-2];
        bf.bytes[0] = bytes[index-1];
    } else {
        bf.bytes[0] = bytes[index-4];
        bf.bytes[1] = bytes[index-3];
        bf.bytes[2] = bytes[index-2];
        bf.bytes[3] = bytes[index-1];}
    return bf.f;
}

double DataView::readFixed() {
    return (double)readInt()/(1 << 16);
}

unsigned int DataView::readDate() {
    unsigned int macTime = readUInt() * 0x100000000 + readUInt();
    unsigned int utc = macTime * 1000; // TODO: + date 1904, 1, 1
    return utc;
}

unsigned int DataView::readUInt(unsigned int offset) {
    if (endian) {
        return ((bytes[offset] & 0xff) << 24) | ((bytes[offset+1] & 0xff) << 16) | ((bytes[offset+2] & 0xff) << 8) | (bytes[offset+3] & 0xff);
    } else {
        return (bytes[offset] & 0xff) | ((bytes[offset+1] & 0xff) << 8) | ((bytes[offset+2] & 0xff) << 16) | ((bytes[offset+3] & 0xff) << 24);
    }
}

int DataView::readInt(unsigned int offset) {
    if (endian) {
        return ((bytes[offset] & 0xff) << 24) | ((bytes[offset+1] & 0xff) << 16) | ((bytes[offset+2] & 0xff) << 8) | (bytes[offset+3] & 0xff);
    } else {
        return (bytes[offset] & 0xff) | ((bytes[offset+1] & 0xff) << 8) | ((bytes[offset+2] & 0xff) << 16) | ((bytes[offset+3] & 0xff) << 24);
    }
}

int DataView::readShort(unsigned int offset) {
    if (endian) {
        return ((bytes[offset] & 0xff) << 8) | (bytes[offset+1] & 0xff);
    } else {
        return (bytes[offset] & 0xff) | ((bytes[offset+1] & 0xff) << 8);
    }
}

int DataView::readByte(unsigned int offset) {
    return (int)(bytes[offset] & 0xff);
}

std::string DataView::readString(unsigned int offset, unsigned int length, bool nullTerminated) {
    std::string buffer;
    for (auto i = 0; i < length; i++) {
        if (nullTerminated && (bytes[offset+i] == 0))
            break;
        buffer += (char)(bytes[offset+i] & 0xff);
    }
    return buffer;
}

float DataView::readFloat(unsigned int offset) {
    bytes2Float bf{};
    if (endian) {
        bf.bytes[3] = bytes[offset];
        bf.bytes[2] = bytes[offset+1];
        bf.bytes[1] = bytes[offset+2];
        bf.bytes[0] = bytes[offset+3];
    } else {
        bf.bytes[0] = bytes[offset];
        bf.bytes[1] = bytes[offset+1];
        bf.bytes[2] = bytes[offset+2];
        bf.bytes[3] = bytes[offset+3];}
    return bf.f;
}

/**
 * Sets the index of the array to offset
 * @param offset
 */
void DataView::seek(unsigned int offset) {
    this->index = offset;
}

void DataView::skip(unsigned int offset) {
    index += offset;
}

unsigned int DataView::getOffset() {
    return index;
}

void DataView::setEndian(bool isBigEndian) {
    this->endian = isBigEndian;
}

DataView DataView::getBufferSlice(unsigned int offset, unsigned int length) {
    // TODO Optimize
    std::vector<unsigned char> buffer;
    length = length > bytes.size() ? bytes.size() : length;
    buffer.reserve(length);
    for (auto i = 0; i < length; i++) {
        buffer.emplace_back(bytes[offset+i]);
    }
    return {buffer, this->endian};
}

std::vector<unsigned char> DataView::getData() {
    return this->bytes;
}