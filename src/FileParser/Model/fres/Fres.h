
#pragma once
#define FSKL_ROT_MODE_QUATERNION 0x00 << 12
#define FSKL_ROT_MODE_EULER_XYZ 0x01 << 12

#include <map>
#include "../../File.h"
#include "../../../Util/LogHelperBE.h"

struct ResourceDict {
    std::vector<std::string> names;
    uint32_t offset;
};

struct Fvtx_Attrib {
    std::string name;
    uint32_t format;
    unsigned short offset;
    unsigned char bufferIndex;
};

struct Fvtx_Buffer {
    uint32_t size;
    std::vector<unsigned char> data;
    uint32_t stride;
    uint32_t divisor;
};

struct FsklBone {
    std::string name;
    float posX, posY, posZ;
    float scaleX, scaleY, scaleZ;
    bool isQuaternion;
    float rotX, rotY, rotZ, rotW = 0.0f;
};

struct Fskl {
    std::vector<FsklBone> bones;
};

struct Fvtx {
    std::vector<Fvtx_Attrib> attribs;
    std::vector<Fvtx_Buffer> vertexBuffer;
};

struct Fshp {
    std::string name;
};

struct Fmat {
    std::string name;
};

struct Fmdl {
    std::string name;
    Fskl skeleton;
    std::vector<Fvtx> vertices;
    std::vector<Fshp> shapes;
    std::vector<Fmat> materials;
};

class Fres : public File {
private:
    std::string name;
    std::vector<Fmdl> models;
    // TODO change return type to void
    void parseFMDL(uint32_t offset, DataView& memoryPoolBuffer, const std::string& compName);
    Fskl parseFSKL(uint32_t offset);
    Fvtx parseFVTX(uint32_t offset, DataView& memoryPoolBuffer);
    Fshp parseFSHP(uint32_t offset, DataView& memoryPoolBuffer);
    Fmat parseFMAT(uint32_t offset);
public:
    explicit Fres(File& file);

    void parse();
    std::string readBinaryString(uint32_t offset);
    ResourceDict parseResourceDicIndex(uint32_t index);
};
