
#include "LogHelperBE.h"

// TODO use singleton instance with file writing
void LogHelperBE::message(const std::string &type, const std::string &msg, bool isError) {
    if (isError) {
        std::cerr << "[BakeEngine]:[" << messageName.back() << "]:[" << type << "] " << msg << std::endl;
    } else {
        std::cout << "[BakeEngine]:[" << messageName.back() << "]:[" << type << "] " << msg << std::endl;
    }
    LogHelperBE::getOutStream() << "[BakeEngine]:[" << messageName.back() << "]:[" << type << "] " << msg << std::endl;
}

void LogHelperBE::fatal(const std::string& message) {
    std::cerr << "[BakeEngine]:[" << messageName.back() << "]:[Fatal Error] " << message << std::endl;
    LogHelperBE::getOutStream() << "[BakeEngine]:[" << messageName.back() << "]:[Fatal Error] " << message << std::endl;
    exit(-1);
}

void LogHelperBE::error(const std::string& message) {
    std::cerr << "[BakeEngine]:[" << messageName.back() << "]:[Error] " << message << std::endl;
    LogHelperBE::getOutStream() << "[BakeEngine]:[" << messageName.back() << "]:[Error] " << message << std::endl;
}

void LogHelperBE::warning(const std::string& message) {
    std::cout << "[BakeEngine]:[" << messageName.back() << "]:[Warning] " << message << std::endl;
    LogHelperBE::getOutStream() << "[BakeEngine]:[" << messageName.back() << "]:[Warning] " << message << std::endl;
}

void LogHelperBE::info(const std::string& message) {
    std::cout << "[BakeEngine]:[" << messageName.back() << "]:[Info] " << message << std::endl;
    LogHelperBE::getOutStream() << "[BakeEngine]:[" << messageName.back() << "]:[Info] " << message << std::endl;
}

void LogHelperBE::pushName(const char *name) {
    messageName.emplace_back(name);
}

void LogHelperBE::popName() {
    messageName.pop_back();
}

LogHelperBE::LogHelperBE() : ooofStream() {
    ooofStream.open("latest-log.txt", std::ofstream::out | std::ofstream::trunc);
}

LogHelperBE::~LogHelperBE() {
    ooofStream.close();
}
