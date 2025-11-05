#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream>

class Node {
public:
    Node() {}
    ~Node() {}

    void printName() const {
        std::cout << "Kat: " << name << std::endl;
    }


private:
    const std::string name = "Node";
};

#endif
