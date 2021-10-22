
#pragma once
#include <iostream>
#include <vector>
#include <fstream>

/**
 * Use it for printing text to the console. Add a new log name before printing (pushName()) and delete it when you are done (popName())
 */

static std::vector<const char*> messageName;

class LogHelperBE {
public:
    LogHelperBE(const LogHelperBE& other) = delete;
    void operator=(const LogHelperBE& other) = delete;

    static LogHelperBE& getInstance() {
        static LogHelperBE instance;
        return instance;
    };
    static std::ofstream& getOutStream() {
        return getInstance().ooofStream;
    }
    static void message(const std::string& type, const std::string& msg, bool isError);
    static void fatal(const std::string& message);
    static void error(const std::string& message);
    static void warning(const std::string& message);
    static void info(const std::string& message);
    static void pushName(const char *name);
    static void popName();
private:
    std::ofstream ooofStream;

    LogHelperBE();
    ~LogHelperBE();
};
