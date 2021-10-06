
#include "LogHelperBE.h"

void LogHelperBE::fatal(const char *message) {
    std::cerr << "[BakeEngine]:[" << messageName.back() << "]:[Fatal Error] " << message << std::endl;
    exit(-1);
}

void LogHelperBE::error(const char *message) {
    std::cerr << "[BakeEngine]:[" << messageName.back() << "]:[Error] " << message << std::endl;
}

void LogHelperBE::warning(const char *message) {
    std::cout << "[BakeEngine]:[" << messageName.back() << "]:[Warning] " << message << std::endl;
}

void LogHelperBE::info(const char *message) {
    std::cout << "[BakeEngine]:[" << messageName.back() << "]:[Info] " << message << std::endl;
}

void LogHelperBE::fatal(std::string& message) {
    std::cerr << "[BakeEngine]:[" << messageName.back() << "]:[Fatal Error] " << message << std::endl;
}

void LogHelperBE::error(std::string& message) {
    std::cerr << "[BakeEngine]:[" << messageName.back() << "]:[Error] " << message << std::endl;
}

void LogHelperBE::warning(std::string& message) {
    std::cout << "[BakeEngine]:[" << messageName.back() << "]:[Warning] " << message << std::endl;
}

void LogHelperBE::info(std::string& message) {
    std::cout << "[BakeEngine]:[" << messageName.back() << "]:[Info] " << message << std::endl;
}

void LogHelperBE::pushName(const char *name) {
    messageName.emplace_back(name);
}

void LogHelperBE::popName() {
    messageName.pop_back();
}