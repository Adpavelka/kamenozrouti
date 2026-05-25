#ifndef NODE_H
#define NODE_H

#include <cstdint>
#include <vector>

#include "component.h"
#include "move_arena.h"
#include "utils.h"

struct Node {
    Board board;
    int score;
    uint64_t hash;
    int moveIdx;     // index into MoveArena, -1 if root
    long long key;

    Node();
    Node(Board b, int s, uint64_t h, int moveIdx);
    Node(const Node&) = default;
    Node& operator=(const Node&) = default;

    void addHeuristic(long long future_score);
    void printing(const MoveArena& arena) const;
};

#endif
