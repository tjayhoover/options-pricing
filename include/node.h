#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>

// Forward declaration to avoid circular dependency
struct Variable;

// Represents a node in the computation graph
// Each node corresponds to one operation involving inputs and outputs
// and contains a backward function for gradient computation
struct Node {
    Node() = default;

    std::vector<Variable*> inputs;
    Variable* output;

    std::function<void()> backward_function;
};

#endif
