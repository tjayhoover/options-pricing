#ifndef OPERATORS_H
#define OPERATORS_H

#include <optional>
#include <functional>

struct Variable;
struct Node;

Variable sin(const Variable& var);
Variable cos(const Variable& var);
Variable exp(const Variable& var);

// Helper function to create a node in the computation graph
// For binary operations there is a second optional parameter that can be passed
Variable create_node(double val, const Variable& var1, const std::optional<Variable>& var2, 
                    std::function<void(const Node&)> backward_func);

#endif
