#include "operators.h"
#include "node.h"
#include "variable.h"
#include "tape.h"
#include "variable_data.h"

#include <cmath>
#include <memory>

using namespace std;

// Sine function
Variable sin(const Variable& var) {
    // Compute the result of the sine
    double val = sin(var.value());

    // Define the backward function for the derivative computation
    auto backward_func = [](const Node& node) {
        double dz = node.output->adjoint;
        node.inputs[0].lock()->adjoint += cos(node.inputs[0].lock()->value) * dz;
    };

    // Create a new node in the computation graph
    return create_node(val, var, nullopt, backward_func);
}

// Cosine function
Variable cos(const Variable& var) {
    // Compute the result of the cosine
    double val = cos(var.value());

    // Define the backward function for derivative computation
    auto backward_func = [] (const Node& node) {
        const double dz = node.output->adjoint;
        node.inputs[0].lock()->adjoint += -sin(node.inputs[0].lock()->value) * dz;
    };

    // Create a new node in the computation graph
    return create_node(val, var, nullopt, backward_func);
}

// Exponential function
Variable exp(const Variable& var) {
    // Compute the result of the exponentiation
    double val = exp(var.value());

    // Define the backward function for derivative computation
    auto backward_func = [] (const Node& node) {
        const double dz = node.output->adjoint;
        node.inputs[0].lock()->adjoint += exp(node.inputs[0].lock()->value) * dz;
    };

    // Create a new node in the computation graph
    return create_node(val, var, nullopt, backward_func);
}

// Helper function to create a node in the computation graph
Variable create_node(double val, const Variable& var1, const optional<Variable>& var2, 
                    function <void(const Node&)> backward_func) {
    // This operation creates a new VariableData on the heap.
    Variable output_var = Variable(val);

    // Create a new Node in the computation graph for this operation
    // (Adding the input and output variables to the node.)
    auto node = make_shared<Node>();

    // Store pointer to data for input #1
    node->inputs.push_back(var1.ptr);

    // Store pointer to data for input #2
    if(var2.has_value()) {
        node->inputs.push_back(var2.value().ptr);
    }
        
    node->output = output_var.ptr;

    // Define the backward function for derivative computation
    node->backward_function = backward_func;

    // Push the node raw pointer onto the tape
    Tape::get().push(node);

    return output_var;
}