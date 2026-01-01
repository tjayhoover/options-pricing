#ifndef VARIABLE_H
#define VARIABLE_H

#include <memory>
#include <optional>

struct VariableData;

// Lightweight wrapper around a VariableData pointer
// Objects of type Variable can be copied and should not be created on the heap
// The actual data (VariableData) will be stored on the heap
struct Variable {
    // Constructor (allocates new variableData to the heap with value if provided)
    Variable(std::optional<double> val);

    // Pointer to the variable data which is stored on the heap
    std::shared_ptr<VariableData> ptr;

    // Get value
    double value() const;

    // Get adjoint
    double adjoint() const;

    // Set the adjoint
    void setAdjoint(double adjoint);

    // Set the value
    void setValue(double value);

    // Overloaded operators
    Variable operator*(const Variable& other);
    Variable operator/(const Variable& other);
    Variable operator+(const Variable& other);  
    Variable operator-(const Variable& other);
};

#endif