
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
    seek(0x0c);

    std::vector<Table> tables;
    for (auto i = 0; i < numTables; i++) {
        Table table{};
        table.tag = readString(4);
        table.checksum = readUInt();
        table.offset = readUInt();
        table.length = readUInt();
        tables.emplace_back(table);
        print(table.tag);
    }
}
