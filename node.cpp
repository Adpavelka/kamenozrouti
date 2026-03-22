#include "node.h"

#include <iostream>

Node::Node() : score(0), hash(0), key(0) {}

Node::Node(Board b, int s, uint64_t h, std::vector<int> mv)
    : board(std::move(b)), score(s), hash(h), moves(std::move(mv)), key(s) {}

Node::Node(const Node& o)
    : board(o.board), score(o.score), hash(o.hash),
      moves(o.moves), key(o.key) {}

Node& Node::operator=(const Node &o) {
    if (this != &o) {
        board = o.board;
        score = o.score;
        hash = o.hash;
        moves = o.moves;
        key = o.key;
    }
    return *this;
}

void Node::addHeuristic(long long future_score) {
    key += future_score;
}

void Node::printing() const {
    std::cout << "Best score found: " << score << "\nPlan: ";
    for (int m : moves) {
        auto [r, c] = idxToRC(m);
        std::cout << "[" << r << ", " << c << "] ";
    }
    std::cout << "\n";
}
