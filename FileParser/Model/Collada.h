
#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "../../Util/LogHelperBE.h"

struct Pair {
    std::string key;
    std::string value;
};

struct Element {
    std::string name;
    std::string data;
    std::vector<Pair> pairs;
    std::vector<Element> children;
};

class Collada {
private:
    Element colladaElement;
    static void parseElement(const std::string& elementLine, Element& element);
    static void parsePair(const std::string& pairStr, Pair& pair);
    std::string path;
public:
    explicit Collada(std::string& path);
    void parse();
    static std::vector<std::string> splitString(const std::string& str, const std::string& splitter);
};
