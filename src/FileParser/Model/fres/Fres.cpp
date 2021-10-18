// TODO buffer: endian, offset pointers, etc.
#include <string>
#include "Fres.h"

Fres::Fres(File &file) : File(file) {

}

void Fres::parse() {
    LogHelperBE::pushName("FRES");
    seek(0x00);
    uint32_t endianMarker = readShort(0x0c);
    if (endianMarker == 0xfffe) {
        setEndian(LITTLE_ENDIAN);
    } else if (endianMarker == 0xffef){
        setEndian(BIG_ENDIAN);
    } else {
        LogHelperBE::error("Could not read endian");
        throw std::runtime_error("File endian is corrupted");
    }
/*    uint32_t version = readUInt(0x08);
    uint32_t supportedVersions[] = {
            0x00800000,
            0x00050003
    };
    if ((version != supportedVersions[0]) || (version != supportedVersions[1])) {
        LogHelperBE::error("Version is not supported");
        throw std::exception("Version is not supported");
    }*/
    {
        uint32_t fileNameOffset = readShort(0x20);
        name = readBinaryString(fileNameOffset);

        uint32_t memoryPoolInfoOffset = readUInt(0x90);
        uint32_t memoryPoolSize = memoryPoolInfoOffset != 0 ? readInt(memoryPoolInfoOffset+0x04) : 0;
        uint32_t memoryPoolDataOffset = memoryPoolInfoOffset != 0 ? readUInt(memoryPoolInfoOffset+0x08) : 0;
        DataView memoryPoolBuffer = memoryPoolInfoOffset != 0 ? getBufferSlice(memoryPoolDataOffset, memoryPoolSize) : DataView(endian);

        ResourceDict fmdlTable = parseResourceDicIndex(0x00);
        uint32_t fmdlTableIndex = fmdlTable.offset;

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

std::string Fres::readBinaryString(uint32_t offset) {
    return readString(offset+0x02, 0xff, true);
}

ResourceDict Fres::parseResourceDicIndex(uint32_t index) {
    ResourceDict buffer{};
    uint32_t arrayOffset = readUInt(0x28 + index*0x10);
    uint32_t resourceDicOffset = readUInt(0x30 + index*0x10);
    buffer.offset = arrayOffset;

    if (resourceDicOffset == 0) {
        return buffer;
    }

    if (readUInt(resourceDicOffset) != 0x00000000) {
        LogHelperBE::error("Signature is corrupted!");
    }
    {
        uint32_t tableCount = readUInt(resourceDicOffset+0x04);

        uint32_t resourceDicTableIdx = resourceDicOffset + 0x18;
        for (auto i = 0; i < tableCount; i++) {
            buffer.names.emplace_back(readBinaryString(readUInt(resourceDicTableIdx+0x08)));
            resourceDicTableIdx += 0x10;
        }
    }
    return buffer;
}

void Fres::parseFMDL(uint32_t offset, DataView& memoryPoolBuffer, const std::string& compName) {
    Fmdl model{};
    model.name = readBinaryString(readUInt(offset + 0x10));
    uint32_t sklOffset = readUInt(offset + 0x20);
    uint32_t vtxArrayOffset = readUInt(offset + 0x28);
    uint32_t shpArrayOffset = readUInt(offset + 0x30);
    uint32_t shapeDicOffset = readUInt(offset + 0x38);
    uint32_t matArrayOffset = readUInt(offset + 0x40);
    uint32_t matDicOffset = readUInt(offset + 0x48);
    uint32_t usrDataArrayOffset = readUInt(offset + 0x50);
    uint32_t usrDataDicOffset = readUInt(offset + 0x58);

    uint32_t usrDataPtr = readUInt(offset + 0x60);
    uint32_t vtxCount = readShort(offset + 0x68);
    uint32_t shpCount = readShort(offset + 0x6a);
    uint32_t matCount = readShort(offset + 0x6c);
    uint32_t usrDataCount = readShort(offset + 0x6e);
    uint32_t totalProcessVtx = readUInt(offset + 0x70);

    model.skeleton = parseFSKL(sklOffset);

    uint32_t vertexArrayIndex = vtxArrayOffset;
    for (auto i = 0; i < vtxCount; i++) {
        if (readString(vertexArrayIndex, 0x04, false) != "FVTX")
            LogHelperBE::error("FVTX offset is corrupted");
        model.vertices.emplace_back(parseFVTX(vertexArrayIndex, memoryPoolBuffer));
        vertexArrayIndex += 0x60;
    }

    uint32_t shpArrayIndex = shpArrayOffset;
    for (auto i = 0; i < shpCount; i++) {
        if (readString(shpArrayIndex, 0x04, false) != "FSHP")
            LogHelperBE::error("FSHP offset is corrupted");
        model.shapes.emplace_back(parseFSHP(shpArrayIndex, memoryPoolBuffer));
        shpArrayIndex += 0x70;
    }

    uint32_t matArrayIndex = matArrayOffset;
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

Fskl Fres::parseFSKL(uint32_t offset) {
    Fskl skeleton{};
    if (readString(offset, 0x04, false) != "FSKL") {
        LogHelperBE::error("FSKL offset is corrupted!");
    }
    {
        uint32_t boneArrayOffset = readUInt(offset+0x18);
        uint32_t flag = readUInt(offset+0x48);
        uint32_t boneCount = readShort(offset+0x4c);
        uint32_t smoothMtxCount = readShort(offset+0x4e);
        uint32_t rigidMtxCount = readShort(offset+0x50);

        uint32_t boneArrayIdx = boneArrayOffset;
        for (auto i = 0; i < boneCount; i++) {
            FsklBone bone{};
            bone.name = readBinaryString(readUInt(boneArrayIdx));
            uint32_t index = readShort(boneArrayIdx+0x28);
            uint32_t parentIndex = readShort(boneArrayIdx+0x2a);
            uint32_t smoothMtxIndex = readShort(boneArrayIdx+0x2c);
            uint32_t rigidMtxIndex = readShort(boneArrayIdx+0x2e);
            uint32_t billboardIndex = readShort(boneArrayIdx+0x30);
            uint32_t boneFlag = readUInt(boneArrayIdx+0x34);

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

Fvtx Fres::parseFVTX(uint32_t offset, DataView& memoryPoolBuffer) {
    Fvtx vertex{};

    uint32_t vtxAttribArrayOffset = readUInt(offset+0x10);
    uint32_t vtxBufferInfoArrayOffset = readUInt(offset+0x38);
    uint32_t vtxBufferStateInfoArrayOffset = readUInt(offset+0x40);
    uint32_t memoryPoolOffset = readUInt(offset+0x50);
    uint32_t vtxAttribCount = readByte(offset+0x54);
    uint32_t vtxBufferCount = readByte(offset+0x55);

    uint32_t vtxAttribArrayIndex = vtxAttribArrayOffset;
    for (auto i = 0; i < vtxAttribCount; i++) {
        Fvtx_Attrib attrib{};
        attrib.name = readBinaryString(readUInt(vtxAttribArrayIndex));
        attrib.format = readUInt(vtxAttribArrayIndex+0x08);
        attrib.offset = readShort(vtxAttribArrayIndex+0x0c);
        attrib.bufferIndex = readByte(vtxAttribArrayIndex+0x0e);
        vertex.attribs.emplace_back(attrib);
        vtxAttribArrayIndex += 0x10;
    }

    uint32_t vtxBufferInfoArrayIndex = vtxBufferInfoArrayOffset;
    uint32_t vtxBufferStateInfoArrayIndex = vtxBufferStateInfoArrayOffset;
    uint32_t memoryPoolRunningOffset = memoryPoolOffset;
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

Fshp Fres::parseFSHP(uint32_t offset, DataView& memoryPoolBuffer) {
    // TODO Some work?
    Fshp shape{};
    shape.name = readBinaryString(readUInt(offset+0x10));
    uint32_t meshArrayOffset = readUInt(offset+0x20);
    uint32_t boundingBoxArrayOffset = readUInt(offset+0x40);

    uint32_t matIndex = readShort(offset+0x5e);
    uint32_t boneIndex = readShort(offset+0x60);
    uint32_t vtxIndex = readShort(offset+0x62);

    uint32_t meshCount = readByte(offset+0x67);

    return shape;
}

Fmat Fres::parseFMAT(uint32_t offset) {
    Fmat mat{};
    mat.name = readBinaryString(readUInt(offset+0x10));
    uint32_t renderInfoArrayOffset = readUInt(offset + 0x18);
    uint32_t shaderAssignOffset = readUInt(offset+0x28);
    uint32_t textureArrayOffset = readUInt(offset+0x30);
    uint32_t textureNameOffset = readUInt(offset+0x38);
    uint32_t samplerInfoArrayOffset = readUInt(offset+0x48);
    uint32_t samplerInfoDictOffset = readUInt(offset+0x50);
    uint32_t shaderParamArrayOffset = readUInt(offset+0x58);
    uint32_t srcParamOffset = readUInt(offset+0x68);
    uint32_t userDataArrayOffset = readUInt(offset+0x70);
    uint32_t flag = readUInt(offset+0xa0);
    uint32_t index = readShort(offset+0xa4);
    uint32_t renderInfoCount = readShort(offset+0xa6);
    uint32_t samplerCount = readByte(offset+0xa8);
    uint32_t textureCount = readByte(offset+0xa9);
    uint32_t shaderParamCount = readShort(offset+0xaa);
    uint32_t shaderParamVolatileCount = readShort(offset+0xac);
    uint32_t srcParamCount = readShort(offset+0xae);
    uint32_t rawParamCount = readShort(offset+0xb0);
    uint32_t userDataCount = readShort(offset+0xb2);

    uint32_t renderInfoArrayIndex = renderInfoArrayOffset;

    return mat;
}