
#include <map>
#include "TrueType.h"

void print(unsigned int a) {
    LogHelperBE::pushName("TTF");
    LogHelperBE::info(std::to_string(a).c_str());
    LogHelperBE::popName();
}

void print(std::string& a) {
    LogHelperBE::pushName("TTF");
    LogHelperBE::info(a);
    LogHelperBE::popName();
}

TrueType::TrueType(const std::string &path) : File(path, BIG_ENDIAN) {
    unsigned int version = readUInt();
    if ((version != 0x00010000) && (version != 0x4f54544f)) {
        throw std::runtime_error("Font version is not supported!");
    }
    unsigned int numTables = readShort();
    // Skip unnecessary information
    skip(0x06);

    std::map<std::string, Table> tables;
    for (auto i = 0; i < numTables; i++) {
        Table table{};
        table.tag = readString(4);
        table.checksum = readUInt();
        table.offset = readUInt();
        table.length = readUInt();
        tables.insert(std::make_pair(table.tag, table));
    }
    readHead(tables["head"].offset);
    readMaxp(tables["maxp"].offset);
    readHhea(tables["hhea"].offset);
    Hmtx hmtx{};
    for (auto i = 0; i < hhea.numOfLongHorMetrics; i++) {
        HMetric metric{};
        metric.advanceWidth = readShort();
        metric.leftSideBearing = readShort();
        hmtx.metrics.emplace_back(metric);
    }
    hmtx.leftSideBearing.reserve(maxp.numGlyphs - hhea.numOfLongHorMetrics);
    for (unsigned int i = 0; i < maxp.numGlyphs - hhea.numOfLongHorMetrics; i++) {
        int lsb = readInt();
        hmtx.leftSideBearing.emplace_back(lsb);
    }
    unsigned int location = head.indexToLocFormat == 0 ? readShort() : readUInt();
    std::vector<unsigned int> locations;
    locations.reserve(maxp.numGlyphs + 1);
    for (auto i = 0; i < maxp.numGlyphs + 1; i++) {
        locations.emplace_back(location);
    }
    std::vector<Glyf> glyphs;
    // TODO Fix
    for (unsigned int loca : locations) {
        unsigned int multiplier = head.indexToLocFormat == 0 ? 2 : 1;
        unsigned int locationOffset = loca * multiplier;
        skip(locationOffset);
        Glyf glyf{};
        glyf.numberOfContours = readShort();
        glyf.xMin = readShort();
        glyf.yMin = readShort();
        glyf.xMax = readShort();
        glyf.yMax = readShort();
        glyphs.emplace_back(glyf);
    }
}

void TrueType::readHead(unsigned int offset) {
    unsigned int old = getOffset();
    seek(offset);
    head.majorVersion = readShort();
    head.minorVersion = readShort();
    head.fontRevision = readFixed();
    head.checksumAdjustment = readUInt();
    head.magicNumber = readUInt();
    if (head.magicNumber != 0x5f0f3cf5) {
        LogHelperBE::pushName("TTF");
        LogHelperBE::error("Head magic is wrong!");
        LogHelperBE::popName();
    }
    head.flags = readShort();
    head.unitsPerEm = readShort();
    skip(0x10); // skip dates, 2 * 8 bytes
    head.xMin = readInt();
    head.yMin = readInt();
    head.xMax = readInt();
    head.yMax = readInt();
    head.macStyle = readShort();
    head.lowestRecPPEM = readShort();
    head.fontDirectionHint = readShort();
    head.indexToLocFormat = readShort();
    head.glyphDataFormat = readShort();
    seek(old);
}

void TrueType::readMaxp(unsigned int offset) {
    unsigned int old = getOffset();
    seek(offset);
    maxp.version = readFixed();
    maxp.numGlyphs = readShort();
    maxp.maxPoints = readShort();
    maxp.maxContours = readShort();
    maxp.maxCompositePoints = readShort();
    maxp.maxCompositeContours = readShort();
    maxp.maxZones = readShort();
    maxp.maxTwilightPoints = readShort();
    maxp.maxStorage = readShort();
    maxp.maxFunctionDefs = readShort();
    maxp.maxInstructionDefs = readShort();
    maxp.maxStackElements = readShort();
    maxp.maxSizeOfInstructions = readShort();
    maxp.maxComponentElements = readShort();
    maxp.maxComponentDepth = readShort();
    seek(old);
}

void TrueType::readHhea(unsigned int offset) {
    unsigned int old = getOffset();
    seek(offset);
    hhea.version = readFixed();
    hhea.ascent = readInt();
    hhea.descent = readInt();
    hhea.lineGap = readInt();
    hhea.advanceWidthMax = readUInt();
    hhea.minLeftSideBearing = readInt();
    hhea.minRightSideBearing = readInt();
    hhea.xMaxExtent = readInt();
    hhea.caretSlopeRise = readShort();
    hhea.caretSlopeRun = readShort();
    hhea.caretOffset = readInt();
    skip(0x08);
    hhea.metricDataFormat = readShort();
    hhea.numOfLongHorMetrics = readShort();
    seek(old);
}
