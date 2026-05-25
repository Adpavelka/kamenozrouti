#include "node.h"

#include <iostream>

Node::Node() : score(0), hash(0), moveIdx(-1), key(0) {}

Node::Node(Board b, int s, uint64_t h, int mi)
    : board(std::move(b)), score(s), hash(h), moveIdx(mi), key(s) {}

void Node::addHeuristic(long long future_score) {
    key += future_score;
}

void Node::printing(const MoveArena& arena) const {
    std::cout << "Best score found: " << score << "\nPlan: ";
    auto path = arena.reconstruct(moveIdx);
    for (int m : path) {
        auto [r, c] = idxToRC(m);
        std::cout << "[" << r << "," << c << "] ";
    }
    std::cout << "\n";
}
