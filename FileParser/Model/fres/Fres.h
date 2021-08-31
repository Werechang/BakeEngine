
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

};

struct Fshp {

};

struct Fmat {

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
    static void p(const char* str);
    static void p(std::string& str);
    ResourceDict parseResourceDicIndex(unsigned int index);
    Fmdl parseFMDL(unsigned int offset, DataView& memoryPoolBuffer);
    Fskl parseFSKL(unsigned int offset);
};

#endif
