#include "Application/Application.h"
#include "Scene/Model.h"

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

int main(int argc, char* args[]) {
    LogHelperBE::pushName("Main");
    // Hide window if on Windows
    char buffer[PATH_MAX_LEN];
    #ifdef _WIN32
    // Hide console
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    // Copy exe path to buffer
    GetModuleFileNameA(nullptr, buffer, PATH_MAX_LEN);
    #elif __APPLE__
    // Copy exe path to buffer
    if (!_NSGetExecutablePath(buffer, PATH_MAX_LEN))
        LogHelperBE::fatal("Path buffer is too small! Please move the executable and resources into a different location");
    if (!realpath(buffer, buffer))
        LogHelperBE::fatal("A pathname caused a problem. Please change the path(name) of the executable and resources");
    #elif __LINUX__
    // Copy exe path to buffer
    if (readlink("proc/self/exe", buffer, PATH_MAX_LEN) == -1)
        LogHelperBE::fatal("Something went wrong while reading the path.");
    #endif
    std::string pathBuffer(buffer);
    // Replace windows \ with standardized /
    std::replace(pathBuffer.begin(), pathBuffer.end(), '\\', '/');
    // Erase ever char from the last / to get rid of /BakeEngine.exe (or similar)
    pathBuffer.erase(pathBuffer.find_last_of('/'), pathBuffer.back());
    // search for the resources folder
    pathBuffer += "/resources/";
    LogHelperBE::info(pathBuffer);
    // Get the working directory
    Application::absolutePath = pathBuffer;

    // For testing purposes. Will be removed later
    Model m("models/output.bfres");
    m.load();

    // Launching the application
    Application(true, 1600, 900, "BakeEngine").start();

    LogHelperBE::popName();
    return 0;
}
