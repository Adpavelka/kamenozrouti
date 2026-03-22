#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <vector>
#include "board.h"

struct Zobrist {
    std::vector<uint64_t> table;
    uint64_t salt;
    Zobrist();
    uint64_t hash(const Board &b) const;
};

#endif
