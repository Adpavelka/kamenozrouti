#ifndef COMPONENT_H
#define COMPONENT_H

#include <vector>
#include "board.h"

inline int rcToIndex(int r, int c) {
    return r * W + c;
}

inline std::pair<int,int> idxToRC(int idx) {
    return { idx / W, idx % W };
}

struct Dir {
    int dr, dc;
};

static const Dir DIRS[4] = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1}
};


struct Component {
    int color;
    int size;
    std::vector<int> cells;

    Component() = default;

    Component(int color, int size, std::vector<int>&& cells ) :
        color{color},
        size{size},
        cells{std::move(cells)} {
    }
};

std::vector<Component> extractComponents(const Board &b);
Board applyMove(const Board &b, const Component &comp);

#endif
