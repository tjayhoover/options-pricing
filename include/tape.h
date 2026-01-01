#ifndef TAPE_H
#define TAPE_H

#include <vector>
#include <memory>

struct Node;

// Represents the tape that records the sequence of operations (nodes)
struct Tape {
    std::vector<std::shared_ptr<Node>> nodes;

    // Singleton access method
    static Tape& get() {
        static Tape instance; // initialized on first use with the private constructor
        return instance;
    }

    // Push a node to the tape
    void push(const std::shared_ptr<Node>& node) {
        nodes.push_back(node);
    }

    // Execture the backward pass through the tape
    // This performs a synchronous backward propagation step to compute adjoints
    void execute_backpass() {
        for(auto iter = nodes.rbegin(); iter != nodes.rend(); ++iter) {
            iter->get()->execute_backward();
        }
    }

private:
    Tape() = default;               // private constructor
    ~Tape() = default;
    Tape(const Tape&) = delete;     // no copy
    Tape& operator=(const Tape&) = delete; // no assignment

};

#endif