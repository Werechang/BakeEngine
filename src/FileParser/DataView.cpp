
#include "DataView.h"

DataView::DataView(std::vector<unsigned char> &data, bool endian) : bytes(data), endian(endian){
}

DataView::DataView(bool endian) : bytes(std::vector<unsigned char>()), endian(endian) {
}

uint32_t DataView::readUInt() {
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

std::string DataView::readString(uint32_t length) {
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

uint32_t DataView::readDate() {
    uint32_t macTime = readUInt() * 0x100000000 + readUInt();
    uint32_t utc = macTime * 1000; // TODO: + date 1904, 1, 1
    return utc;
}

uint32_t DataView::readUInt(uint32_t offset) {
    if (endian) {
        return ((bytes[offset] & 0xff) << 24) | ((bytes[offset+1] & 0xff) << 16) | ((bytes[offset+2] & 0xff) << 8) | (bytes[offset+3] & 0xff);
    } else {
        return (bytes[offset] & 0xff) | ((bytes[offset+1] & 0xff) << 8) | ((bytes[offset+2] & 0xff) << 16) | ((bytes[offset+3] & 0xff) << 24);
    }
}

int DataView::readInt(uint32_t offset) {
    if (endian) {
        return ((bytes[offset] & 0xff) << 24) | ((bytes[offset+1] & 0xff) << 16) | ((bytes[offset+2] & 0xff) << 8) | (bytes[offset+3] & 0xff);
    } else {
        return (bytes[offset] & 0xff) | ((bytes[offset+1] & 0xff) << 8) | ((bytes[offset+2] & 0xff) << 16) | ((bytes[offset+3] & 0xff) << 24);
    }
}

int DataView::readShort(uint32_t offset) {
    if (endian) {
        return ((bytes[offset] & 0xff) << 8) | (bytes[offset+1] & 0xff);
    } else {
        return (bytes[offset] & 0xff) | ((bytes[offset+1] & 0xff) << 8);
    }
}

int DataView::readByte(uint32_t offset) {
    return (int)(bytes[offset] & 0xff);
}

std::string DataView::readString(uint32_t offset, uint32_t length, bool nullTerminated) {
    std::string buffer;
    for (auto i = 0; i < length; i++) {
        if (nullTerminated && (bytes[offset+i] == 0))
            break;
        buffer += (char)(bytes[offset+i] & 0xff);
    }
    return buffer;
}

float DataView::readFloat(uint32_t offset) {
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
void DataView::seek(uint32_t offset) {
    this->index = offset;
}

void DataView::skip(uint32_t offset) {
    index += offset;
}

uint32_t DataView::getOffset() {
    return index;
}

void DataView::setEndian(bool isBigEndian) {
    this->endian = isBigEndian;
}

DataView DataView::getBufferSlice(uint32_t offset, uint32_t length) {
    // TODO Optimize, with std::span?
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