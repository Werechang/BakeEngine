
#include "InputNode.h"
#include "../Util/LogHelperBE.h"

InputNode::InputNode(int key) : key(key) {

}

void InputNode::checkActive() const {
    if (KeyboardKeys[key]) {
        for (auto & function : staticFunctions) {
            function->operator()();
        }
        for (auto & pair : nonStaticFunctions) {
            pair.first->operator()(*pair.second);
        }
        for (auto && child : children) {
            child->checkActive();
        }
    }
}

void InputNode::addStaticFunction(std::function<void()> *function) {
    staticFunctions.emplace_back(function);
}

void InputNode::addNonStaticFunction(std::function<void(InputCallable&)> *function, InputCallable *obj) {
    if (!obj) {
        LogHelperBE::pushName("InputNode");
        LogHelperBE::error("Object is nullptr!");
        LogHelperBE::popName();
        return;
    }
    nonStaticFunctions.insert(std::make_pair(function, obj));
}

void InputNode::addChild(InputNode* node) {
    if (node && !node->parent && (this->parent != node)) {
        children.emplace_back(node);
        node->parent = this;
    } else {
        LogHelperBE::pushName("InputNode");
        LogHelperBE::error("Node does already have a parent!");
        LogHelperBE::popName();
    }
}

int InputNode::getKey() const {
    return key;
}
