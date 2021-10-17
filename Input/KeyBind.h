
#pragma once

#include <map>
#include <utility>
#include <vector>
#include <functional>
#include "../Util/LogHelperBE.h"

class KeyBind {
public:
    KeyBind() = default;
    KeyBind(int mods, const std::function<void()>& function);

    void checkActive(int mods);
    void addFunction(int mods, const std::function<void()>& function);
private:
    std::unordered_map<int, std::function<void()>> modsFunctions;
};
