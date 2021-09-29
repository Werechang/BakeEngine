
#ifndef BAKEENGINE_FRES_H
#define BAKEENGINE_FRES_H
#define FSKL_ROT_MODE_QUATERNION 0x00 << 12
#define FSKL_ROT_MODE_EULER_XYZ 0x01 << 12

#include <map>
#include "../../File.h"
#include "../../../Util/LogHelperBE.h"

struct ResourceDict {
    std::vector<std::string> names;
    unsigned int offset;
};

struct Fvtx_Attrib {
    std::string name;
    unsigned int format;
    unsigned short offset;
    unsigned char bufferIndex;
};

struct Fvtx_Buffer {
    unsigned int size;
    std::vector<unsigned char> data;
    unsigned int stride;
    unsigned int divisor;
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

class Fres {
private:
    File file;
    std::string name;
    std::vector<Fmdl> models;
public:
    explicit Fres(File& file) : file(file) {};

    void parse();
    std::string readBinaryString(unsigned int offset);
    static void p(std::string& str);
    ResourceDict parseResourceDicIndex(unsigned int index);
    Fmdl parseFMDL(unsigned int offset, DataView& memoryPoolBuffer);
    Fskl parseFSKL(unsigned int offset);
    Fvtx parseFVTX(unsigned int offset, DataView& memoryPoolBuffer);
    Fshp parseFSHP(unsigned int offset, DataView& memoryPoolBuffer);
    Fmat parseFMAT(unsigned int offset);
};

#endif
