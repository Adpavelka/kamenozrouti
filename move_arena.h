#ifndef MOVE_ARENA_H
#define MOVE_ARENA_H

#include <vector>
#include <mutex>
#include <algorithm>

struct MoveArena {
    struct Entry { int move, parent; };

    std::vector<Entry> nodes;
    std::mutex mtx;

    MoveArena() { nodes.reserve(1 << 22); } // ~4M entries upfront

    int add(int move, int parent) {
        std::lock_guard<std::mutex> lk(mtx);
        int idx = (int)nodes.size();
        nodes.push_back({move, parent});
        return idx;
    }

    std::vector<int> reconstruct(int idx) const {
        std::vector<int> path;
        while (idx >= 0) {
            path.push_back(nodes[idx].move);
            idx = nodes[idx].parent;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
};

#endif