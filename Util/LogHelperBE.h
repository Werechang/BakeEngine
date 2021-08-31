
#ifndef BAKEENGINE_LOGHELPERBE_H
#define BAKEENGINE_LOGHELPERBE_H

#include <iostream>
#include <vector>

/**
 * Use it for printing text to the console. Add a new log name before printing (pushName()) and delete it when you are done (popName())
 */

static std::vector<const char*> messageName;

class LogHelperBE {
public:
    static void fatal(const char* message);
    static void error(const char* message);
    static void warning(const char* message);
    static void info(const char* message);
    static void pushName(const char* name);
    static void popName();
};


#endif //BAKEENGINE_LOGHELPERBE_H
