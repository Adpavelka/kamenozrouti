#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <thread>

static constexpr int H = 10;           
static constexpr int W = 20;           
static constexpr int COLORS = 5;       
static constexpr int BEAM_WIDTH = 200000;
static constexpr int MAX_CHILDREN = 800000;

static const int THREADS =
    std::thread::hardware_concurrency() ?
    std::thread::hardware_concurrency() : 4;

static const uint64_t ZOBRIST_SEED = 0x9e3779b97f4a7c15ULL;

#endif
