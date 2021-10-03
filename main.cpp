#include "Application/Application.h"
#include "Scene/Model.h"

std::string Application::absolutePath;

int main(int argc, char* args[]) {
    LogHelperBE::pushName("Main");

    Application::absolutePath = std::filesystem::current_path().string() + "/resources/";
    std::replace(Application::absolutePath.begin(), Application::absolutePath.end(), '\\', '/');

    Model m("models/output.bfres");
    m.load();

    Application(true, 1600, 900, "BakeEngine").start();

    LogHelperBE::popName();
    return 0;
}
