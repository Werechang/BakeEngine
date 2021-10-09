
#ifndef BAKEENGINE_INPUTCALLABLE_H
#define BAKEENGINE_INPUTCALLABLE_H

#include "../Util/LogHelperBE.h"

class InputCallable {
public:
    static void testInput() {
        LogHelperBE::info("Input test successful (Static)");
    }

    void testInputObj() {
        LogHelperBE::info("Input test successful (Object)");
    }
    ~InputCallable() {
        LogHelperBE::info("Input callable destructor. Is every KeyBind function call deleted?");
    };
};

#endif
