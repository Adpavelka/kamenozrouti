#include "component.h"
#include <algorithm>

static std::vector<int> floodFill(const Board &b, int startIdx,
                                  std::array<int, H * W>& visited, int mark)
{
    const int startColor = b.a[startIdx];
    std::vector<int> q;
    q.reserve(64);

    visited[startIdx] = mark;
    q.push_back(startIdx);

    for (size_t qi = 0; qi < q.size(); ++qi) {
        int cur = q[qi];
        auto [r, c] = idxToRC(cur);

        for (const auto &d : DIRS) {
            int nr = r + d.dr;
            int nc = c + d.dc;
            if (nr < 0 || nr >= H || nc < 0 || nc >= W) {
                continue;
            }

            int ni = rcToIndex(nr, nc);
            if (!visited[ni] && b.a[ni] == startColor) {
                visited[ni] = mark;
                q.push_back(ni);
            }
        }
    }
    return q;
}


static bool isColumnEmpty(const Board &b, int c)
{
    for (int r {}; r < H; ++r) {
        if (b.at(r, c) >= 0) {
            return false;
        }
    }
    return true;
}

static void compressDown(Board& b) {
    for (int c {}; c < W; ++c) {
        int write {H - 1};
        for (int r = {H - 1}; r >= 0; --r) {
            int idx = rcToIndex(r,c);
            if (b.a[idx] >= 0) {
                b.a[rcToIndex(write,c)] = b.a[idx];
                if (write != r) {
                    b.a[idx] = -1;
                }
                --write;
            }
        }
        for (int r {write}; r >= 0; --r) {
            b.a[rcToIndex(r,c)] = -1;
        }
    }
}

static void compressColumns(Board& b) {
    int writecol {};
    for (int c {}; c < W; ++c) {
        if (!isColumnEmpty(b, c)) {
            if (writecol != c) {
                for (int r {}; r < H; ++r) {
                    b.at(r, writecol) = b.at(r,c);
                    b.at(r, c) = -1;
                }
            }
            ++writecol;
        }
    }

    for (int c {writecol}; c < W; ++c) {
        for (int r {}; r < H; ++r) {
            b.at(r,c) = -1;
        }
    }
}


std::vector<Component> extractComponents(const Board &b)
{
    std::vector<Component> comps;

    std::array<int, H * W> visited{};
    visited.fill(0);
    int mark = 1;

    for (int idx = 0; idx < H * W; ++idx) {
        if (b.a[idx] < 0 || visited[idx]) {
            continue;
        }

        auto cells = floodFill(b, idx, visited, mark++);
        if (cells.size() >= 2) {
            comps.emplace_back(b.a[idx], cells.size(), std::move(cells));
        }
    }
    return comps;
}

Board applyMove(const Board &b, const Component &comp) {
    Board nb = b;

    for (auto idx : comp.cells) {
        nb.a[idx] = -1;
    }

    compressDown(nb);
    compressColumns(nb);

    return nb;
}
