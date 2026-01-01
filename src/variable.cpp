#include "variable.h"
#include "node.h"
#include "variable_data.h"
#include "tape.h"
#include "operators.h"

#include <string>
#include <iostream>
#include <memory>
#include <cmath>
#include <vector>
#include <functional>
#include <optional>

using namespace std;

// Variable constructor (allocates new VariableData on the heap)
Variable::Variable(optional<double> value) {
        // Allocate a new VariableData on the heap
        ptr = make_shared<VariableData>();
        if (value.has_value()) {
            ptr->value = value.value();
        }
    }

// Set the adjoint
void Variable::setAdjoint(double adjoint) {
    this->ptr->adjoint = adjoint;
}

// Set the variable
void Variable::setValue(double variable) {
    this->ptr->value = variable;
}

// Overloaded multiplication operator
Variable Variable::operator*(const Variable& other) {

    // Compute the result of the multiplication
    double val = this->value() * other.value();

    // Define the backward function for derivative computation
    auto backward_func = [] (const Node& node) {
        const double dz = node.output->adjoint;
        node.inputs[0].lock()->adjoint += node.inputs[1].lock()->value * dz;
        node.inputs[1].lock()->adjoint += node.inputs[0].lock()->value * dz;
    };

    // Create a new node in the computation graph
    return create_node(val, *this, other, backward_func);
}

// Overloaded division operator
Variable Variable::operator/(const Variable& other) {

    // Compute the result of the division
    double val = this->value() / other.value();

    // Define the backward function for derivative computation
    auto backward_func = [] (const Node& node) {
        const double dz = node.output->adjoint;
        double u = node.inputs[0].lock()->value;
        double v = node.inputs[1].lock()->value;
        node.inputs[0].lock()->adjoint += (1.0 / v) * dz;
        node.inputs[1].lock()->adjoint += (-u / (v * v)) * dz;
    };

    // Create a new node in the computation graph
    return create_node(val, *this, other, backward_func);
}

// Overloaded addition operator
Variable Variable::operator+(const Variable& other) {
    // Compute the result of the addition
    double val = this->value() + other.value();

    // Define the backward function for derivative computation
    auto backward_func = [] (const Node& node) {
        const double dz = node.output->adjoint;
        node.inputs[0].lock()->adjoint += dz;
        node.inputs[1].lock()->adjoint += dz;
    };

    // Create a new node in the computation graph
    return create_node(val, *this, other, backward_func);
}

// Overloaded subtraction operator
Variable Variable::operator-(const Variable& other) {
    // Compute the result of the subtraction
    double val = this->value() - other.value();

    // Define the backward function for derivative computation
    auto backward_func = [] (const Node& node) {
        const double dz = node.output->adjoint;
        node.inputs[0].lock()->adjoint += dz;
        node.inputs[1].lock()->adjoint += -dz;
    };

    // Create a new node in the computation graph
    return create_node(val, *this, other, backward_func);
}

// Get value
double Variable::value() const { return ptr.get()->value; }

// Get adjoint
double Variable::adjoint() const { return ptr.get()->adjoint; }