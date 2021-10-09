#ifndef BAKEENGINE_TRUETYPE_H
#define BAKEENGINE_TRUETYPE_H

#include "File.h"

struct Table {
    std::string tag;
    unsigned int offset;
    unsigned int checksum;
    unsigned int length;
};

struct head {
    unsigned int majorVersion;
    unsigned int minorVersion;
    // Fixed point value. char is just a placeholder
    float fontRevision;
    unsigned int checksumAdjustment;
    unsigned int magicNumber;
    unsigned int flags;
    unsigned int unitsPerEm;
    // created and modified: dates
    int xMin;
    int yMin;
    int xMax;
    int yMax;
    unsigned int macStyle;
    unsigned int lowestRecPPEM;
    unsigned int fontDirectionHint;
    unsigned int indexToLocFormat;
    unsigned int glyphDataFormat;
};

struct maxp {
    // version is fixed point
    float version;
    unsigned int numGlyphs;
    unsigned int maxPoints;
    unsigned int maxContours;
    unsigned int maxCompositePoints;
    unsigned int maxCompositeContours;
    unsigned int maxZones;
    unsigned int maxTwilightPoints;
    unsigned int maxStorage;
    unsigned int maxFunctionDefs;
    unsigned int maxInstructionDefs;
    unsigned int maxStackElements;
    unsigned int maxSizeOfInstructions;
    unsigned int maxComponentElements;
    unsigned int maxComponentDepth;
};

class TrueType : public File {
private:
public:
    explicit TrueType(const std::string &path);
};

#endif