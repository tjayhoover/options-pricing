#include <iostream>

#include "node.h"
#include "tape.h"
#include "variable.h"

using namespace std;

int main() {
    Tape tape = Tape();

    cout << "Number of nodes in tape: " << tape.nodes.size() << endl;
    
    Variable x = Variable();
    Variable y = Variable();

    x.tape = &tape;
    y.tape = &tape;

    x.value = 3.0;
    y.value = 4.0;
    auto result = x * y;

    cout << result << endl;

    cout << "Number of nodes in tape after operation: " << tape.nodes.size() << endl;

    Node* node = tape.nodes[0];
    cout << "Number of inputs in the first node: " << node->inputs.size() << endl;
    node->backward_function();

    return 0;
}