
#include "LogHelperBE.h"

void LogHelperBE::fatal(const std::string& message) {
    std::cerr << "[BakeEngine]:[" << messageName.back() << "]:[Fatal Error] " << message << std::endl;
}

void LogHelperBE::error(const std::string& message) {
    std::cerr << "[BakeEngine]:[" << messageName.back() << "]:[Error] " << message << std::endl;
}

void LogHelperBE::warning(const std::string& message) {
    std::cout << "[BakeEngine]:[" << messageName.back() << "]:[Warning] " << message << std::endl;
}

void LogHelperBE::info(const std::string& message) {
    std::cout << "[BakeEngine]:[" << messageName.back() << "]:[Info] " << message << std::endl;
}

void LogHelperBE::pushName(const char *name) {
    messageName.emplace_back(name);
}

void LogHelperBE::popName() {
    messageName.pop_back();
}