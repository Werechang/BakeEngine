// TODO buffer: endian, offset pointers, etc.
#include <string>
#include "Fres.h"

Fres::Fres(File &file) : File(file) {

}

void Fres::parse() {
    LogHelperBE::pushName("FRES");
    seek(0x00);
    unsigned int endianMarker = readShort(0x0c);
    if (endianMarker == 0xfffe) {
        setEndian(LITTLE_ENDIAN);
    } else if (endianMarker == 0xffef){
        setEndian(BIG_ENDIAN);
    } else {
        LogHelperBE::error("Could not read endian");
        throw std::runtime_error("File endian is corrupted");
    }
/*    unsigned int version = readUInt(0x08);
    unsigned int supportedVersions[] = {
            0x00800000,
            0x00050003
    };
    if ((version != supportedVersions[0]) || (version != supportedVersions[1])) {
        LogHelperBE::error("Version is not supported");
        throw std::exception("Version is not supported");
    }*/
    {
        unsigned int fileNameOffset = readShort(0x20);
        name = readBinaryString(fileNameOffset);

        unsigned int memoryPoolInfoOffset = readUInt(0x90);
        unsigned int memoryPoolSize = memoryPoolInfoOffset != 0 ? readInt(memoryPoolInfoOffset+0x04) : 0;
        unsigned int memoryPoolDataOffset = memoryPoolInfoOffset != 0 ? readUInt(memoryPoolInfoOffset+0x08) : 0;
        DataView memoryPoolBuffer = memoryPoolInfoOffset != 0 ? getBufferSlice(memoryPoolDataOffset, memoryPoolSize) : DataView(endian);

        ResourceDict fmdlTable = parseResourceDicIndex(0x00);
        unsigned int fmdlTableIndex = fmdlTable.offset;

        for (auto & mdlName : fmdlTable.names) {
            if (readString(fmdlTableIndex, 0x04, false) != "FMDL")
                LogHelperBE::error("FMDL is corrupted");
            parseFMDL(fmdlTableIndex, memoryPoolBuffer, mdlName);
            fmdlTableIndex += 0x78;
        }

        LogHelperBE::info("Parsed file successfully");
        LogHelperBE::popName();
    }
}

std::string Fres::readBinaryString(unsigned int offset) {
    return readString(offset+0x02, 0xff, true);
}

ResourceDict Fres::parseResourceDicIndex(unsigned int index) {
    ResourceDict buffer{};
    unsigned int arrayOffset = readUInt(0x28 + index*0x10);
    unsigned int resourceDicOffset = readUInt(0x30 + index*0x10);
    buffer.offset = arrayOffset;

    if (resourceDicOffset == 0) {
        return buffer;
    }

    if (readUInt(resourceDicOffset) != 0x00000000) {
        LogHelperBE::error("Signature is corrupted!");
    }
    {
        unsigned int tableCount = readUInt(resourceDicOffset+0x04);

        unsigned int resourceDicTableIdx = resourceDicOffset + 0x18;
        for (auto i = 0; i < tableCount; i++) {
            buffer.names.emplace_back(readBinaryString(readUInt(resourceDicTableIdx+0x08)));
            resourceDicTableIdx += 0x10;
        }
    }
    return buffer;
}

void Fres::parseFMDL(unsigned int offset, DataView& memoryPoolBuffer, const std::string& compName) {
    Fmdl model{};
    model.name = readBinaryString(readUInt(offset + 0x10));
    unsigned int sklOffset = readUInt(offset + 0x20);
    unsigned int vtxArrayOffset = readUInt(offset + 0x28);
    unsigned int shpArrayOffset = readUInt(offset + 0x30);
    unsigned int shapeDicOffset = readUInt(offset + 0x38);
    unsigned int matArrayOffset = readUInt(offset + 0x40);
    unsigned int matDicOffset = readUInt(offset + 0x48);
    unsigned int usrDataArrayOffset = readUInt(offset + 0x50);
    unsigned int usrDataDicOffset = readUInt(offset + 0x58);

    unsigned int usrDataPtr = readUInt(offset + 0x60);
    unsigned int vtxCount = readShort(offset + 0x68);
    unsigned int shpCount = readShort(offset + 0x6a);
    unsigned int matCount = readShort(offset + 0x6c);
    unsigned int usrDataCount = readShort(offset + 0x6e);
    unsigned int totalProcessVtx = readUInt(offset + 0x70);

    model.skeleton = parseFSKL(sklOffset);

    unsigned int vertexArrayIndex = vtxArrayOffset;
    for (auto i = 0; i < vtxCount; i++) {
        if (readString(vertexArrayIndex, 0x04, false) != "FVTX")
            LogHelperBE::error("FVTX offset is corrupted");
        model.vertices.emplace_back(parseFVTX(vertexArrayIndex, memoryPoolBuffer));
        vertexArrayIndex += 0x60;
    }

    unsigned int shpArrayIndex = shpArrayOffset;
    for (auto i = 0; i < shpCount; i++) {
        if (readString(shpArrayIndex, 0x04, false) != "FSHP")
            LogHelperBE::error("FSHP offset is corrupted");
        model.shapes.emplace_back(parseFSHP(shpArrayIndex, memoryPoolBuffer));
        shpArrayIndex += 0x70;
    }

    unsigned int matArrayIndex = matArrayOffset;
    for (auto i = 0; i < matCount; i++) {
        if (readString(matArrayIndex, 0x04, false) != "FMAT")
            LogHelperBE::error("FMAT offset is corrupted");
        model.materials.emplace_back(parseFMAT(matArrayIndex));
        matArrayIndex += 0xb8;
    }
    if (model.name != compName) {
        LogHelperBE::error("FMDL names are different");
    } else {
        models.emplace_back(model);
    }
}

Fskl Fres::parseFSKL(unsigned int offset) {
    Fskl skeleton{};
    if (readString(offset, 0x04, false) != "FSKL") {
        LogHelperBE::error("FSKL offset is corrupted!");
    }
    {
        unsigned int boneArrayOffset = readUInt(offset+0x18);
        unsigned int flag = readUInt(offset+0x48);
        unsigned int boneCount = readShort(offset+0x4c);
        unsigned int smoothMtxCount = readShort(offset+0x4e);
        unsigned int rigidMtxCount = readShort(offset+0x50);

        unsigned int boneArrayIdx = boneArrayOffset;
        for (auto i = 0; i < boneCount; i++) {
            FsklBone bone{};
            bone.name = readBinaryString(readUInt(boneArrayIdx));
            unsigned int index = readShort(boneArrayIdx+0x28);
            unsigned int parentIndex = readShort(boneArrayIdx+0x2a);
            unsigned int smoothMtxIndex = readShort(boneArrayIdx+0x2c);
            unsigned int rigidMtxIndex = readShort(boneArrayIdx+0x2e);
            unsigned int billboardIndex = readShort(boneArrayIdx+0x30);
            unsigned int boneFlag = readUInt(boneArrayIdx+0x34);

            bone.scaleX = readFloat(boneArrayIdx+0x38);
            bone.scaleY = readFloat(boneArrayIdx+0x3c);
            bone.scaleZ = readFloat(boneArrayIdx+0x40);
            if ((boneFlag & FSKL_ROT_MODE_EULER_XYZ)) {
                bone.isQuaternion = false;
                bone.rotX = readFloat(boneArrayIdx + 0x44);
                bone.rotY = readFloat(boneArrayIdx + 0x48);
                bone.rotZ = readFloat(boneArrayIdx + 0x4C);
            } else {
                bone.isQuaternion = true;
                bone.rotX = readFloat(boneArrayIdx + 0x44);
                bone.rotY = readFloat(boneArrayIdx + 0x48);
                bone.rotZ = readFloat(boneArrayIdx + 0x4C);
                bone.rotW = readFloat(boneArrayIdx + 0x50);
            }
            bone.posX = readFloat(boneArrayIdx + 0x54);
            bone.posY = readFloat(boneArrayIdx + 0x58);
            bone.posZ = readFloat(boneArrayIdx + 0x5C);

            skeleton.bones.emplace_back(bone);
            boneArrayIdx += 0x60;
        }
    }
    return skeleton;
}

Fvtx Fres::parseFVTX(unsigned int offset, DataView& memoryPoolBuffer) {
    Fvtx vertex{};

    unsigned int vtxAttribArrayOffset = readUInt(offset+0x10);
    unsigned int vtxBufferInfoArrayOffset = readUInt(offset+0x38);
    unsigned int vtxBufferStateInfoArrayOffset = readUInt(offset+0x40);
    unsigned int memoryPoolOffset = readUInt(offset+0x50);
    unsigned int vtxAttribCount = readByte(offset+0x54);
    unsigned int vtxBufferCount = readByte(offset+0x55);

    unsigned int vtxAttribArrayIndex = vtxAttribArrayOffset;
    for (auto i = 0; i < vtxAttribCount; i++) {
        Fvtx_Attrib attrib{};
        attrib.name = readBinaryString(readUInt(vtxAttribArrayIndex));
        attrib.format = readUInt(vtxAttribArrayIndex+0x08);
        attrib.offset = readShort(vtxAttribArrayIndex+0x0c);
        attrib.bufferIndex = readByte(vtxAttribArrayIndex+0x0e);
        vertex.attribs.emplace_back(attrib);
        vtxAttribArrayIndex += 0x10;
    }

    unsigned int vtxBufferInfoArrayIndex = vtxBufferInfoArrayOffset;
    unsigned int vtxBufferStateInfoArrayIndex = vtxBufferStateInfoArrayOffset;
    unsigned int memoryPoolRunningOffset = memoryPoolOffset;
    for (auto i = 0; i < vtxBufferCount; i++) {
        Fvtx_Buffer buffer{};
        buffer.size = readUInt(vtxBufferInfoArrayIndex);
        buffer.stride = readUInt(vtxBufferStateInfoArrayIndex);
        buffer.divisor = readUInt(vtxBufferStateInfoArrayIndex + 0x04);
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
    // TODO Some work?
    Fshp shape{};
    shape.name = readBinaryString(readUInt(offset+0x10));
    unsigned int meshArrayOffset = readUInt(offset+0x20);
    unsigned int boundingBoxArrayOffset = readUInt(offset+0x40);

    unsigned int matIndex = readShort(offset+0x5e);
    unsigned int boneIndex = readShort(offset+0x60);
    unsigned int vtxIndex = readShort(offset+0x62);

    unsigned int meshCount = readByte(offset+0x67);

    return shape;
}

Fmat Fres::parseFMAT(unsigned int offset) {
    Fmat mat{};
    mat.name = readBinaryString(readUInt(offset+0x10));
    unsigned int renderInfoArrayOffset = readUInt(offset + 0x18);
    unsigned int shaderAssignOffset = readUInt(offset+0x28);
    unsigned int textureArrayOffset = readUInt(offset+0x30);
    unsigned int textureNameOffset = readUInt(offset+0x38);
    unsigned int samplerInfoArrayOffset = readUInt(offset+0x48);
    unsigned int samplerInfoDictOffset = readUInt(offset+0x50);
    unsigned int shaderParamArrayOffset = readUInt(offset+0x58);
    unsigned int srcParamOffset = readUInt(offset+0x68);
    unsigned int userDataArrayOffset = readUInt(offset+0x70);
    unsigned int flag = readUInt(offset+0xa0);
    unsigned int index = readShort(offset+0xa4);
    unsigned int renderInfoCount = readShort(offset+0xa6);
    unsigned int samplerCount = readByte(offset+0xa8);
    unsigned int textureCount = readByte(offset+0xa9);
    unsigned int shaderParamCount = readShort(offset+0xaa);
    unsigned int shaderParamVolatileCount = readShort(offset+0xac);
    unsigned int srcParamCount = readShort(offset+0xae);
    unsigned int rawParamCount = readShort(offset+0xb0);
    unsigned int userDataCount = readShort(offset+0xb2);

    unsigned int renderInfoArrayIndex = renderInfoArrayOffset;

    return mat;
}