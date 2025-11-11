#include "variable.h"

#include <string>
#include <iostream>
#include <memory>
#include <cmath>
#include <vector>
#include <functional>

using namespace std;

// Overloaded multiplication operator
double Variable::operator*(const Variable& other) {
    // Compute the result of the operation
    double val = this->value * other.value;

    // Create the output Variable to hold the result
    auto output = new Variable();
    output->value = val;

    // Create a new Node in the computation graph for this operation
    // (Adding the input and output variables to the node.)
    this->node = new Node();
    
    // create input 1 as a fresh Variable and copy the scalar value
    auto in1 = new Variable();
    in1->value = this->value;
    this->node->inputs.push_back(in1);
    
    // create input 2 as a fresh Variable and copy the scalar value
    auto in2 = new Variable();
    in2->value = other.value;
    this->node->inputs.push_back(in2);
    
    this->node->output = output;

    // Define the backward function for derivative computation
    this->node->backward_function = ([] {cout << "Backward" << endl;});

    // Push the node onto the tape
    tape->nodes.push_back(this->node);

    return val;
}

// Overloaded addition operator
double Variable::operator+(const Variable& other) {
    return this->value + other.value;
}
// Overloaded subtraction operator
double Variable::operator-(const Variable& other) {
    double output = this->value - other.value;
    return output;
}
// Sine function
double Variable::sin() {
    return std::sin(this->value);
}
// Cosine function
double Variable::cos() {
    return std::cos(this->value);
}
// Exponential function
double Variable::exp() {
    return std::exp(this->value);
}
