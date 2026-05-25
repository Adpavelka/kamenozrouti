#ifndef SOLVER_H
#define SOLVER_H

#include <future>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "move_arena.h"
#include "node.h"
#include "thread_pool.h"
#include "zobrist.h"

class Solver {
    int beamWidth;
    ThreadPool pool;
    Zobrist Z;
    MoveArena arena;

    std::vector<Node> expandNode(const Node& st,
                                 std::unordered_map<uint64_t,int>& tt,
                                 std::mutex& tt_mutex);

    void integrateResults(std::vector<Node>& dst,
                          std::future<std::vector<Node>>& fut);

    void pruneBeam(std::vector<Node>& all);
    void updateBest(Node& curBest, const std::vector<Node>& beam);

    void exactSolve(const Board& b, int score, int parentIdx, int& bestScore, int& bestMoveIdx);

public:
    Solver(int beamWidth, int threads);
    Node solve(const Board &start);

    const MoveArena& getArena() const {
        return arena;
    }
};

#endif
