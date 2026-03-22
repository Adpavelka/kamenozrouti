#ifndef NODE_H
#define NODE_H

#include <cstdint>
#include <vector>

#include "component.h"
#include "utils.h"

struct Node {
    Board board;
    int score;
    uint64_t hash;
    std::vector<int> moves;
    long long key;

    Node();
    Node(Board b, int s, uint64_t h, std::vector<int> mv);
    Node(const Node &other);
    Node& operator=(const Node &other);

    void addHeuristic(long long future_score);
    void printing() const;
};

#endif
