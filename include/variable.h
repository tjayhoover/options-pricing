#ifndef VARIABLE_H
#define VARIABLE_H

#include "node.h"
#include "tape.h"

struct Variable {
    double adjoint = 0.0;
    double value;
    Node* node;
    Tape* tape;

    Variable() : value(0.0), node(nullptr), tape(nullptr) {}

    // Overloaded multiplication operator
    double operator*(const Variable& other);
    
    // Overloaded addition operator
    double operator+(const Variable& other);  // Overloaded subtraction operator
    double operator-(const Variable& other);
    // Sine function
    double sin();
    double cos();

    // Exponential function
    double exp();
};

#endif