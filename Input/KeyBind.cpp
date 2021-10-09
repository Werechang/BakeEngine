
#include "KeyBind.h"
#include "../Util/LogHelperBE.h"

std::map<int, std::unique_ptr<KeyBind>> KeyBind::keyBinds;

KeyBind::KeyBind(int mods, const std::function<void()>& function) {
    addFunction(mods, function);
}

KeyBind::KeyBind(int mods, const std::function<void(InputCallable &)>& function, InputCallable *obj) {
    addFunction(mods, function, obj);
}

void KeyBind::checkActive(int mods) const {
    for (const auto& pair : modsFunctions) {
        if (pair.first == mods) {
            pair.second->execute();
        }
    }
}

void KeyBind::addFunction(int mods, const std::function<void()>& function) {
    if (modsFunctions.contains(mods) || !function) {
        LogHelperBE::pushName("KeyBind");
        LogHelperBE::error("Mods are already used or function is nullptr!");
        LogHelperBE::popName();
    } else {
        modsFunctions.insert(std::make_pair(mods, std::make_unique<KeyBindFunctionStaticNoArgs>(function)));
    }
}

void KeyBind::addFunction(int mods, const std::function<void(InputCallable &)>& function, InputCallable *obj) {
    if (modsFunctions.contains(mods) || !obj || !function) {
        LogHelperBE::pushName("KeyBind");
        LogHelperBE::error("Mods are already used or object or function is nullptr!");
        LogHelperBE::popName();
    } else {
        modsFunctions.insert(std::make_pair(mods, std::make_unique<KeyBindFunctionObjectNoArgs>(function, obj)));
    }
}
