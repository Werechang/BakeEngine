#include "Application/Application.h"
#include "Scene/Model.h"
#include "FileParser/TrueType.h"

#ifdef _WIN32
#include <Windows.h>
#elif __APPLE__
#include <stdlib.h>
#elif __LINUX__
#include <libgen.h>
#include <unistd.h>
#endif
#define PATH_MAX_LEN 1024

std::string Application::absolutePath;
static uint64_t usage = 0;

void* operator new(size_t size) {
    usage += size;
    return malloc(size);
}

void operator delete(void* obj, size_t size) {
    usage -= size;
    free(obj);
}

void printMemUsage() {
    std::cout << "[BakeEngine]:[Memory]:[Heap] Usage: " << usage << " bytes" << std::endl;
}

int main(int argc, char* args[]) {
    // TODO reduce number of static variables
    // TODO replace unsigned int with uint32_t and long long with int64_t
    {
        LogHelperBE::pushName("Main");
        // Hide window if on Windows
        char buffer[PATH_MAX_LEN];
    #ifdef _WIN32
        // Hide console
        ShowWindow(GetConsoleWindow(), SW_HIDE);
        // Copy exe path to buffer
        // TODO Error check
        GetModuleFileNameA(nullptr, buffer, PATH_MAX_LEN);
    #elif __APPLE__
        // Copy exe path to buffer
        char cpyBuf[PATH_MAX_LEN];
        uint32_t bufSize = sizeof(cpyBuf);
        if (_NSGetExecutablePath(cpyBuf, &bufSize) != 0)
            LogHelperBE::fatal("Path buffer is too small! Please move the executable and resources into a different location");
        if (!realpath(cpyBuf, buffer))
            LogHelperBE::fatal("A pathname caused a problem. Please change the path(name) of the executable and resources");
    #elif __LINUX__
        // Copy exe path to buffer
        if (readlink("/proc/self/exe", buffer, PATH_MAX_LEN) == -1)
            LogHelperBE::fatal("Something went wrong while reading the path.");
    #endif
        std::string pathBuffer(buffer);
        // Replace windows \ with standardized /
        std::replace(pathBuffer.begin(), pathBuffer.end(), '\\', '/');
        // Erase ever char from the last / to get rid of /BakeEngine.exe (or similar)
        pathBuffer.erase(pathBuffer.find_last_of('/'), pathBuffer.back());
        // search for the resources folder
        pathBuffer += "/resources/";
        // Get the working directory
        Application::absolutePath = pathBuffer;

        // For testing purposes. Will be removed later
        //Model m("models/simple.dae");
        //m.load();
        //TrueType ttf("fonts/consola.ttf");

        // Launching the application
        printMemUsage();
        Application app(true, 1600, 900, "BakeEngine");
        app.start();
        printMemUsage();

        LogHelperBE::popName();
    }
    printMemUsage();
    return 0;
}
