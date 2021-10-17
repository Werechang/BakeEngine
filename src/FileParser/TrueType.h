#pragma once

#include "File.h"

struct Table {
    std::string tag;
    unsigned int offset;
    unsigned int checksum;
    unsigned int length;
};

struct Head {
    unsigned int majorVersion;
    unsigned int minorVersion;
    double fontRevision;
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

struct Maxp {
    double version;
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

struct Hhea {
    double version;
    unsigned int ascent;
    unsigned int descent;
    unsigned int lineGap;
    unsigned int advanceWidthMax;
    int minLeftSideBearing;
    int minRightSideBearing;
    int xMaxExtent;
    int caretSlopeRise;
    int caretSlopeRun;
    int caretOffset;
    int metricDataFormat;
    unsigned int numOfLongHorMetrics;
};

struct HMetric {
    unsigned int advanceWidth;
    int leftSideBearing;
};

struct Hmtx {
    std::vector<HMetric> metrics;
    std::vector<int> leftSideBearing;
};

struct Glyf {
    int numberOfContours;
    int xMin;
    int yMin;
    int xMax;
    int yMax;
};

class TrueType : public File {
private:
    Head head;
    Maxp maxp;
    Hhea hhea;
public:
    explicit TrueType(const std::string &path);

    void readHead(unsigned int offset);
    void readMaxp(unsigned int offset);
    void readHhea(unsigned int offset);
};
