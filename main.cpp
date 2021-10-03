#include "Application/Application.h"
#include "Scene/Model.h"

#ifdef _WIN32
#include <Windows.h>
#endif

std::string Application::absolutePath;

int main(int argc, char* args[]) {
    // Hide window if on Windows
    #ifdef _WIN32
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    #endif

    LogHelperBE::pushName("Main");

    // Get the working directory
    Application::absolutePath = std::filesystem::current_path().string() + "/resources/";
    std::replace(Application::absolutePath.begin(), Application::absolutePath.end(), '\\', '/');

    // For testing purposes. Will be removed later
    Model m("models/output.bfres");
    m.load();

    // Launching the application
    Application(true, 1600, 900, "BakeEngine").start();

    LogHelperBE::popName();
    return 0;
}
