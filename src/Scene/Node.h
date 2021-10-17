
#pragma once

#include <vector>
#include "Mesh.h"

class Node {
private:
    std::vector<Node> children;
    std::vector<Mesh*> meshes;
};
