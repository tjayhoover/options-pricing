#include <iostream>

#include "node.h"
#include "tape.h"
#include "variable_data.h"
#include "variable.h"
#include "operators.h"

using namespace std;

int main() { 
    // Create the input variables
    Variable x = Variable(3.0);
    Variable y = Variable(4.0);

    // Write the expression (implicitly builds a computation graph)
    Variable z = sin(x + y) * exp(x / y) * cos(x * y);

    cout << "Number of nodes in tape after operation: " << Tape::get().nodes.size() << endl;

    // Set the adjoint of the output variable to 1.0 to start backpropagation
    z.setAdjoint(1.0);

    // Execute the backward pass to compute the gradients
    Tape::get().execute_backpass();

    cout << "Adjoints:" << endl;
    cout << "dz/dx: " << x.adjoint() << endl;
    cout << "dz/dy: " << y.adjoint() << endl;

    cout << "Number of inputs in the first node: " << Tape::get().nodes[0]->inputs.size() << endl;
    return 0;
}