#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>


// Forward declaration to avoid circular dependency
struct VariableData;

// Represents a node in the computation graph
// Each node corresponds to one operation involving inputs and outputs
// and contains a backward function for gradient computation
struct Node {
    Node() = default;

    std::vector<std::weak_ptr<VariableData>> inputs;
    std::shared_ptr<VariableData> output;
    std::function<void(const Node&)> backward_function;

    void execute_backward() {
        backward_function(*this);
    }
};

#endif
