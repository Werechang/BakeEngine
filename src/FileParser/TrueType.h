#pragma once

#include "File.h"

struct Table {
    std::string tag;
    uint32_t offset;
    uint32_t checksum;
    uint32_t length;
};

struct Head {
    uint32_t majorVersion;
    uint32_t minorVersion;
    double fontRevision;
    uint32_t checksumAdjustment;
    uint32_t magicNumber;
    uint32_t flags;
    uint32_t unitsPerEm;
    // created and modified: dates
    int xMin;
    int yMin;
    int xMax;
    int yMax;
    uint32_t macStyle;
    uint32_t lowestRecPPEM;
    uint32_t fontDirectionHint;
    uint32_t indexToLocFormat;
    uint32_t glyphDataFormat;
};

struct Maxp {
    double version;
    uint32_t numGlyphs;
    uint32_t maxPoints;
    uint32_t maxContours;
    uint32_t maxCompositePoints;
    uint32_t maxCompositeContours;
    uint32_t maxZones;
    uint32_t maxTwilightPoints;
    uint32_t maxStorage;
    uint32_t maxFunctionDefs;
    uint32_t maxInstructionDefs;
    uint32_t maxStackElements;
    uint32_t maxSizeOfInstructions;
    uint32_t maxComponentElements;
    uint32_t maxComponentDepth;
};

struct Hhea {
    double version;
    uint32_t ascent;
    uint32_t descent;
    uint32_t lineGap;
    uint32_t advanceWidthMax;
    int minLeftSideBearing;
    int minRightSideBearing;
    int xMaxExtent;
    int caretSlopeRise;
    int caretSlopeRun;
    int caretOffset;
    int metricDataFormat;
    uint32_t numOfLongHorMetrics;
};

struct HMetric {
    uint32_t advanceWidth;
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

    void readHead(uint32_t offset);
    void readMaxp(uint32_t offset);
    void readHhea(uint32_t offset);
};
