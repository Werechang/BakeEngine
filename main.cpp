#include "Application/Application.h"
#include "Scene/Model.h"

int main(int argc, char* args[]) {
    LogHelperBE::pushName("Main");



    Model m("../resources/output.bfres");
    m.load();

    //Application(true, 1600, 900, "BakeEngine").start();


    LogHelperBE::popName();
    return 0;
}
