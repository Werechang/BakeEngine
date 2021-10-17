
#include "Collada.h"

#include <utility>
#include "../../Application/Application.h"

Collada::Collada(std::string& path) : path(path) {}

void Collada::parse() {
    LogHelperBE::pushName("Collada");

    std::ifstream input(Application::absolutePath + path);
    std::string line;
    std::getline(input, line);
    if (line != R"(<?xml version="1.0" encoding="utf-8"?>)") {
        LogHelperBE::error("File is corrupted!");
        return;
    }
    // COLLADA element
    std::vector<Element*> parents;
    {
        std::getline(input, line);
        parseElement(line.substr(line.find('<')+1, line.find('>')-1), colladaElement);
    }
    parents.emplace_back(&colladaElement);

    while (std::getline(input, line)) {
        size_t end = line.find('>');
        size_t start = line.find('<');
        if (line[end-1] == '/') {
            // element end, begin in this line <unit ... />
            Element e{};
            // TODO end-2 or end-1?
            parseElement(line.substr(start+1, end-start-2), e);
            parents.back()->children.emplace_back(e);
        } else if ((end+1) != line.size()) {
            // element begin and element end in one line, data in between <author>Blender User</author>
        } else if (line[start+1] == '/') {
            // element end, begin in other line </asset>
            parents.pop_back();
            if (parents.empty()) {
                break;
            }
        } else {
            // element begin, no data <asset> -> next line is child element
            // TODO fix element in parent. reference to dead object?
            Element e{};
            parseElement(line.substr(start+1, end-start-1), e);
            parents.back()->children.emplace_back(e);
            parents.emplace_back(&e);
        }
    }
    LogHelperBE::info("Parsed file successfully");
    LogHelperBE::popName();
}

void Collada::parseElement(const std::string& elementLine, Element& element) {
    std::vector<std::string> nameAndTags = splitString(elementLine, " ");
    if (nameAndTags.empty()) {
        LogHelperBE::error("Element is empty");
    }
    element.name = nameAndTags[0];
    for (auto i = 1; i < nameAndTags.size(); i++) {
        Pair pair{};
        parsePair(nameAndTags[i], pair);
        element.pairs.emplace_back(pair);
    }
}

void Collada::parsePair(const std::string& pairStr, Pair& pair) {
    size_t separateSign = pairStr.find('=');
    pair.key = pairStr.substr(0, separateSign);
    pair.value = pairStr.substr(separateSign+2, pairStr.find_last_of('\"')-separateSign-2);
}

std::vector<std::string> Collada::splitString(const std::string &str, const std::string &splitter) {
    if ((str.size() < splitter.size()) || (str.find(splitter) == std::string::npos)) {
        return {str};
    } else {
        std::vector<std::string> vec;
        std::string temp = str;
        size_t splLen = splitter.length();
        size_t pos;
        while ((pos = temp.find(splitter)) != std::string::npos) {
            if (pos == 0) {
                temp.erase(0, 1);
                continue;
            }
            vec.emplace_back(temp.substr(0, pos));
            temp.erase(0, pos+splLen);
        }
        return vec;
    }
}
