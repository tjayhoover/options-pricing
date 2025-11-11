#ifndef TAPE_H
#define TAPE_H

#include "node.h"
#include <vector>
#include <memory>

// Represents the tape that records the sequence of operations (nodes)
struct Tape {
    std::vector<Node*> nodes;
};

#endif
