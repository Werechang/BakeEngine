// TODO buffer: endian, offset pointers, etc.
#include <string>
#include "Fres.h"

void Fres::parse() {
    LogHelperBE::pushName("FRES");
    unsigned int endianMarker = file.readShort(0x0c);
    if (endianMarker == 0xfffe) {
        file.setEndian(LITTLE_ENDIAN);
    } else if (endianMarker == 0xffef){
        file.setEndian(BIG_ENDIAN);
    } else {
        LogHelperBE::error("Could not read endian");
        throw std::exception("File endian is corrupted");
    }
/*    unsigned int version = file.readUInt(0x08);
    unsigned int supportedVersions[] = {
            0x00800000,
            0x00050003
    };
    if ((version != supportedVersions[0]) || (version != supportedVersions[1])) {
        LogHelperBE::error("Version is not supported");
        throw std::exception("Version is not supported");
    }*/

    unsigned int fileNameOffset = file.readShort(0x20);
    name = readBinaryString(fileNameOffset);
    p(name);

    unsigned int memoryPoolInfoOffset = file.readUInt(0x90);
    unsigned int memoryPoolSize = memoryPoolInfoOffset != 0 ? file.readInt(memoryPoolInfoOffset+0x04) : 0;
    unsigned int memoryPoolDataOffset = memoryPoolInfoOffset != 0 ? file.readUInt(memoryPoolInfoOffset+0x08) : 0;
    DataView memoryPoolBuffer = memoryPoolInfoOffset != 0 ? file.getBufferSlice(memoryPoolDataOffset, memoryPoolSize) : DataView(file.endian);

    ResourceDict fmdlTable = parseResourceDicIndex(0x00);
    unsigned int fmdlTableIndex = fmdlTable.offset;

    for (auto & mdlName : fmdlTable.names) {
        if (file.readString(fmdlTableIndex, 0x04, false) != "FMDL")
            LogHelperBE::error("FMDL is corrupted");
        Fmdl mdl = parseFMDL(fmdlTableIndex, memoryPoolBuffer);
        if (mdl.name != mdlName)
            LogHelperBE::error("FMDL names are different");
        models.emplace_back(mdl);
        fmdlTableIndex += 0x78;
    }

    LogHelperBE::info("Parsed file successfully");
    LogHelperBE::popName();
}

std::string Fres::readBinaryString(unsigned int offset) {
    return file.readString(offset+0x02, 0xff, true);
}

void Fres::p(const char *str) {
    LogHelperBE::info(str);
}

void Fres::p(std::string &str) {
    LogHelperBE::info(str.c_str());
}

ResourceDict Fres::parseResourceDicIndex(unsigned int index) {
    ResourceDict buffer{};
    unsigned int arrayOffset = file.readUInt(0x28 + index*0x10);
    unsigned int resourceDicOffset = file.readUInt(0x30 + index*0x10);
    buffer.offset = arrayOffset;

    if (resourceDicOffset == 0) {
        return buffer;
    }

    if (file.readUInt(resourceDicOffset) != 0x00000000)
        LogHelperBE::error("Signature is corrupted!");
    unsigned int tableCount = file.readUInt(resourceDicOffset+0x04);

    unsigned int resourceDicTableIdx = resourceDicOffset + 0x18;
    for (auto i = 0; i < tableCount; i++) {
        buffer.names.emplace_back(readBinaryString(file.readUInt(resourceDicTableIdx+0x08)));
        resourceDicTableIdx += 0x10;
    }
    return buffer;
}

Fmdl Fres::parseFMDL(unsigned int offset, DataView& memoryPoolBuffer) {
    Fmdl model{};
    // name
    model.name = readBinaryString(file.readUInt(offset + 0x10));
    unsigned int sklOffset = file.readUInt(offset + 0x20);
    unsigned int vtxArrayOffset = file.readUInt(offset + 0x28);
    unsigned int shpArrayOffset = file.readUInt(offset + 0x30);
    unsigned int shapeDicOffset = file.readUInt(offset + 0x38);
    unsigned int matArrayOffset = file.readUInt(offset + 0x40);
    unsigned int matDicOffset = file.readUInt(offset + 0x48);
    unsigned int usrDataArrayOffset = file.readUInt(offset + 0x50);
    unsigned int usrDataDicOffset = file.readUInt(offset + 0x58);

    unsigned int usrDataPtr = file.readUInt(offset + 0x60);
    unsigned int vtxCount = file.readShort(offset + 0x68);
    unsigned int shpCount = file.readShort(offset + 0x6a);
    unsigned int matCount = file.readShort(offset + 0x6c);
    unsigned int usrDataCount = file.readShort(offset + 0x6e);
    unsigned int totalProcessVtx = file.readUInt(offset + 0x70);

    model.skeleton = parseFSKL(sklOffset);

    unsigned int vertexArrayIndex = vtxArrayOffset;
    for (auto i = 0; i < vtxCount; i++) {
        if (file.readString(vertexArrayIndex, 0x04, false) != "FVTX")
            LogHelperBE::error("FVTX offset is corrupted");
        model.vertices.emplace_back(parseFVTX(vertexArrayIndex, memoryPoolBuffer));
        vertexArrayIndex += 0x60;
    }

    unsigned int shpArrayIndex = shpArrayOffset;
    for (auto i = 0; i < shpCount; i++) {
        if (file.readString(shpArrayIndex, 0x04, false) != "FSHP")
            LogHelperBE::error("FSHP offset is corrupted");
        model.shapes.emplace_back(parseFSHP(shpArrayIndex, memoryPoolBuffer));
        shpArrayIndex += 0x70;
    }

    p(model.name);

    return model;
}

Fskl Fres::parseFSKL(unsigned int offset) {
    Fskl skeleton{};
    if (file.readString(offset, 0x04, false) != "FSKL")
        LogHelperBE::error("FSKL offset is corrupted!");

    unsigned int boneArrayOffset = file.readUInt(offset+0x18);
    unsigned int flag = file.readUInt(offset+0x48);
    unsigned int boneCount = file.readShort(offset+0x4c);
    unsigned int smoothMtxCount = file.readShort(offset+0x4e);
    unsigned int rigidMtxCount = file.readShort(offset+0x50);

    unsigned int boneArrayIdx = boneArrayOffset;
    for (auto i = 0; i < boneCount; i++) {
        FsklBone bone{};
        bone.name = readBinaryString(file.readUInt(boneArrayIdx));
        unsigned int index = file.readShort(boneArrayIdx+0x28);
        unsigned int parentIndex = file.readShort(boneArrayIdx+0x2a);
        unsigned int smoothMtxIndex = file.readShort(boneArrayIdx+0x2c);
        unsigned int rigidMtxIndex = file.readShort(boneArrayIdx+0x2e);
        unsigned int billboardIndex = file.readShort(boneArrayIdx+0x30);
        unsigned int boneFlag = file.readUInt(boneArrayIdx+0x34);

        bone.scaleX = file.readFloat(boneArrayIdx+0x38);
        bone.scaleY = file.readFloat(boneArrayIdx+0x3c);
        bone.scaleZ = file.readFloat(boneArrayIdx+0x40);
        if ((boneFlag & FSKL_ROT_MODE_EULER_XYZ)) {
            bone.isQuaternion = false;
            bone.rotX = file.readFloat(boneArrayIdx + 0x44);
            bone.rotY = file.readFloat(boneArrayIdx + 0x48);
            bone.rotZ = file.readFloat(boneArrayIdx + 0x4C);
        } else {
            bone.isQuaternion = true;
            bone.rotX = file.readFloat(boneArrayIdx + 0x44);
            bone.rotY = file.readFloat(boneArrayIdx + 0x48);
            bone.rotZ = file.readFloat(boneArrayIdx + 0x4C);
            bone.rotW = file.readFloat(boneArrayIdx + 0x50);
        }
        bone.posX = file.readFloat(boneArrayIdx + 0x54);
        bone.posY = file.readFloat(boneArrayIdx + 0x58);
        bone.posZ = file.readFloat(boneArrayIdx + 0x5C);

        skeleton.bones.emplace_back(bone);
        boneArrayIdx += 0x60;
    }
    return skeleton;
}

Fvtx Fres::parseFVTX(unsigned int offset, DataView& memoryPoolBuffer) {
    Fvtx vertex{};

    unsigned int vtxAttribArrayOffset = file.readUInt(offset+0x10);
    unsigned int vtxBufferInfoArrayOffset = file.readUInt(offset+0x38);
    unsigned int vtxBufferStateInfoArrayOffset = file.readUInt(offset+0x40);
    unsigned int memoryPoolOffset = file.readUInt(offset+0x50);
    unsigned int vtxAttribCount = file.readByte(offset+0x54);
    unsigned int vtxBufferCount = file.readByte(offset+0x55);

    unsigned int vtxAttribArrayIndex = vtxAttribArrayOffset;
    for (auto i = 0; i < vtxAttribCount; i++) {
        Fvtx_Attrib attrib{};
        attrib.name = readBinaryString(file.readUInt(vtxAttribArrayIndex));
        attrib.format = file.readUInt(vtxAttribArrayIndex+0x08);
        attrib.offset = file.readShort(vtxAttribArrayIndex+0x0c);
        attrib.bufferIndex = file.readByte(vtxAttribArrayIndex+0x0e);
        vertex.attribs.emplace_back(attrib);
        vtxAttribArrayIndex += 0x10;
    }

    unsigned int vtxBufferInfoArrayIndex = vtxBufferInfoArrayOffset;
    unsigned int vtxBufferStateInfoArrayIndex = vtxBufferStateInfoArrayOffset;
    unsigned int memoryPoolRunningOffset = memoryPoolOffset;
    for (auto i = 0; i < vtxBufferCount; i++) {
        Fvtx_Buffer buffer{};
        buffer.size = file.readUInt(vtxBufferInfoArrayIndex);
        buffer.stride = file.readUInt(vtxBufferStateInfoArrayIndex);
        buffer.divisor = file.readUInt(vtxBufferStateInfoArrayIndex + 0x04);
        buffer.data = memoryPoolBuffer.getBufferSlice(memoryPoolRunningOffset, buffer.size).getData();

        vertex.vertexBuffer.emplace_back(buffer);
        // TODO align memoryPoolRunningOffset
        memoryPoolRunningOffset += buffer.size;
        vtxBufferInfoArrayIndex += 0x10;
        vtxBufferStateInfoArrayIndex += 0x10;
    }

    return vertex;
}

Fshp Fres::parseFSHP(unsigned int offset, DataView& memoryPoolBuffer) {
    Fshp shape{};
    shape.name = readBinaryString(file.readUInt(offset+0x10));
    unsigned int meshArrayOffset = file.readUInt(offset+0x20);
    unsigned int boundingBoxArrayOffset = file.readUInt(offset+0x40);

    unsigned int matIndex = file.readShort(offset+0x5e);
    unsigned int boneIndex = file.readShort(offset+0x60);
    unsigned int vtxIndex = file.readShort(offset+0x62);

    unsigned int meshCount = file.readByte(offset+0x67);

    return shape;
}
