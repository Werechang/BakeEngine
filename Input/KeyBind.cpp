
#include "KeyBind.h"

KeyBind::KeyBind(int mods, const std::function<void()>& function) {
    addFunction(mods, function);
}

void KeyBind::checkActive(int mods) {
    if (modsFunctions.find(mods) != modsFunctions.end()) {
        try {
            modsFunctions[mods].operator()();
        } catch (std::bad_function_call&) {
            LogHelperBE::pushName("KeyBind");
            LogHelperBE::error("Function does not exist! Has the object already been destroyed?");
            LogHelperBE::popName();
        }
    }
}

void KeyBind::addFunction(int mods, const std::function<void()>& function) {
    if (modsFunctions.contains(mods) || !function) {
        LogHelperBE::pushName("KeyBind");
        LogHelperBE::error("Mods are already used or function is nullptr!");
        LogHelperBE::popName();
    } else {
        modsFunctions.insert(std::make_pair(mods, function));
    }
}