
#include "InputNode.h"
#include "../Util/LogHelperBE.h"

InputNode::InputNode(int key) : key(key) {

}

bool InputNode::checkActive() const {
    if (KeysAndButtonsPressed[key]) {
        for (auto && child : children) {
            if (child->checkActive())
                return true;
        }
        if (auto ptr = nodeFunction.lock()) {
            ptr->execute();
            return true;
        }
    }
    return false;
}

void InputNode::setFunction(std::function<void()> *function) {
    if (!function) {
        LogHelperBE::pushName("InputNode");
        LogHelperBE::error("Function is nullptr!");
        LogHelperBE::popName();
    } else {
        nodeFunction = std::shared_ptr<AbstractNodeFunction>(new NodeFunctionStaticNoArgs(function));
    }
}

void InputNode::setFunction(std::function<void(InputCallable &)> *function, InputCallable *obj) {
    if (!obj || !function) {
        LogHelperBE::pushName("InputNode");
        LogHelperBE::error("InputCallable object or function is nullptr!");
        LogHelperBE::popName();
    } else {
        nodeFunction = std::shared_ptr<AbstractNodeFunction>(new NodeFunctionObjectNoArgs(function, obj));
    }
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

InputNode *InputNode::getParent() const {
    return parent;
}

bool InputNode::hasFunction() const {
    return !nodeFunction.expired();
}

bool InputNode::singleChild(std::vector<int> &keys) const {
    if (children.empty())
        return true;
    if (children.size() > 1)
        return false;
    int childKey = children.back()->getKey();
    for (auto k : keys) {
        if (childKey == k)
            return true;
    }
    return false;
}
