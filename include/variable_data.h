#ifndef VARIABLE_DATA_H
#define VARIABLE_DATA_H

#include <memory>

struct Node;

struct VariableData {
    double adjoint = 0.0;
    double value;
    std::shared_ptr<Node> node;

    VariableData() : value(0.0), node(nullptr) {}

};

#endif