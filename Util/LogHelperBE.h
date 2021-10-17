
#pragma once
#include <iostream>
#include <vector>

/**
 * Use it for printing text to the console. Add a new log name before printing (pushName()) and delete it when you are done (popName())
 */

static std::vector<const char*> messageName;

class LogHelperBE {
public:
    static void fatal(const std::string& message);
    static void error(const std::string& message);
    static void warning(const std::string& message);
    static void info(const std::string& message);
    static void pushName(const char *name);
    static void popName();
};
