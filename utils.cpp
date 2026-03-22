#include "utils.h"
#include "component.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

int getScore(int n) {
    return (n - 1) * (n - 2) + 2; }

long long heuristicEstimate(const Board &b) {
    std::array<int, COLORS> largest{};
    largest.fill(0);

    std::array<int, H * W> visited{};
    visited.fill(0);
    int mark {1};

    // std::vector<int> comps;

    for (int idx {}; idx < H * W; ++idx) {
        if (b.a[idx] < 0 || visited[idx]) {
            continue;
        }

        int col = b.a[idx];
        std::vector<int> q;
        q.push_back(idx);
        visited[idx] = mark;

        size_t qi {};
        while (qi < q.size()) {
            int cur = q[qi++];
            auto [cr, cc] = idxToRC(cur);
            for (const auto &d : DIRS) {
                int nr = cr + d.dr;
                int nc = cc + d.dc;
                if (nr < 0 || nr >= H || nc < 0 || nc >= W){
                    continue;
                }

                int ni = rcToIndex(nr,nc);
                if (visited[ni] || b.a[ni] != col) {
                    continue;
                }

                visited[ni] = mark;
                q.push_back(ni);
            }
        }
        ++mark;

        largest[col] = std::max(largest[col], (int) q.size());
        // comps.push_back((int)q.size());
    }

    long long est {};
    for (int c {}; c < COLORS; ++c) {
        est += 1LL * largest[c] * largest[c];
    }

    /*
    int limit = std::min<int>(10, (int)comps.size());
    for (int i {}; i < limit; ++i) {
        est += comps[i] * comps[i];
    }
    */

    return est;
}


Board randomBoard(int seed) {
    std::mt19937 rng(seed >= 0 ? seed : (unsigned) std::chrono::high_resolution_clock::now().time_since_epoch().count());

    std::uniform_int_distribution<int> col(0, COLORS - 1);

    Board b;
    for (int r {}; r < H; ++r) {
        for (int c {}; c < W; ++c) {
            b.at(r,c) = col(rng);
        }
    }

    return b;
}

Board initialBoard() {
    Board b;
    b.a = {
        2,2,0,1,2,2,4,2,4,0,4,3,0,0,2,3,4,4,3,3,
        4,4,0,0,3,1,2,0,1,4,2,0,4,0,0,3,1,0,4,3,
        4,4,2,4,3,0,1,3,1,1,4,4,0,3,4,2,3,0,2,1,
        0,3,3,4,4,4,0,1,1,1,1,1,0,4,2,2,1,0,3,4,
        2,4,3,0,4,0,4,0,3,0,3,2,0,4,4,1,2,4,1,2,
        1,4,4,3,4,0,4,0,2,2,3,0,2,1,4,3,0,4,4,2,
        0,0,4,2,1,1,0,1,0,2,0,4,0,4,1,4,1,4,4,3,
        4,4,3,1,1,0,0,3,0,1,0,2,0,4,4,4,4,3,4,4,
        4,4,1,1,3,4,2,3,4,0,3,4,4,2,0,3,3,1,0,4,
        4,1,2,1,0,4,4,1,3,3,3,0,0,3,3,0,0,4,0,4
    };
    return b;
}

Board initialBoard2() {
    Board b;
    b.a = {
        4,2,2,3,0,0,2,4,4,4,4,0,3,4,2,2,3,2,2,0,
        4,2,3,2,3,2,0,2,2,3,2,4,2,3,2,2,2,4,0,2,
        1,2,2,4,0,0,3,3,2,2,2,2,2,3,2,0,0,0,4,2,
        2,2,4,4,1,0,3,4,0,1,2,0,2,2,3,2,2,2,2,2,
        0,1,4,0,1,1,1,0,0,1,1,2,0,3,2,4,0,0,3,1,
        2,1,1,2,2,0,2,2,4,1,4,0,3,4,0,2,2,2,2,1,
        1,3,0,0,3,4,2,3,0,4,1,2,3,1,1,1,4,2,1,3,
        4,4,2,4,0,2,3,3,4,2,0,0,3,2,0,4,4,2,3,0,
        4,4,3,1,4,1,4,4,1,4,0,0,3,3,0,4,0,0,3,2,
        1,3,2,0,1,1,2,3,3,3,2,0,2,1,0,4,4,1,3,2
    };
    return b;
}


Board initialBoard3() {
    Board b;
    b.a = {
        2,3,2,2,0,3,0,0,3,3,0,0,3,1,4,3,0,2,0,1,
        0,0,1,0,0,1,1,0,2,0,2,4,2,0,2,0,1,2,4,2,
        0,0,2,0,0,0,1,0,0,0,3,2,4,0,3,0,1,0,0,2,
        4,0,0,4,3,2,0,2,2,1,3,0,0,3,3,4,1,0,0,0,
        3,1,0,4,1,0,0,4,3,0,0,1,3,4,3,3,0,4,4,2,
        3,3,3,2,2,3,3,4,1,3,0,2,3,0,4,0,1,4,3,3,
        2,1,4,0,0,3,2,0,1,1,0,2,4,0,0,1,3,2,0,2,
        0,0,3,0,4,3,4,4,1,2,2,0,0,4,2,1,1,4,3,0,
        4,4,0,1,3,0,4,3,4,0,1,4,3,0,0,3,2,0,0,1,
        0,2,0,3,0,4,1,3,0,0,0,0,3,0,2,4,0,2,1,0
    };
    return b;
}


void printBoard(const Board &b) {
    for (int r {}; r < H; ++r) {
        for (int c {}; c < W; ++c) {
            int x = b.at(r, c);
            char ch = (x < 0 ? '.' : char('A' + (x % 26)));
            std::cout << ch;
        }
        std::cout << "\n";
    }
}
