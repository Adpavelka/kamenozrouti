#include "zobrist.h"
#include <random>
#include <chrono>

Zobrist::Zobrist() {
    std::mt19937_64 rng(
        (uint64_t)std::chrono::high_resolution_clock::now()
        .time_since_epoch().count() ^ ZOBRIST_SEED
    );

    table.resize(H * W * COLORS);
    for (auto &v : table) {
        v = rng();
    }
    salt = rng();
}

uint64_t Zobrist::hash(const Board &b) const {
    uint64_t h = salt;
    for (int i=0;i<H*W;++i) {
        int col = b.a[i];
        if (col >= 0)
            h ^= table[i * COLORS + col];
    }
    return h;
}
