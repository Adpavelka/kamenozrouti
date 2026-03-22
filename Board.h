#ifndef BOARD_H
#define BOARD_H

#include <array>
#include "config.h"

struct Board {
    std::array<int, H * W> a;
    Board() { a.fill(-1); }

    inline int& at(int r, int c) {
        return a[r * W + c];
    }

    inline int at(int r, int c) const {
        return a[r * W + c]; 
    }

    inline bool isEmpty(int r, int c) const {
        return at(r,c) < 0;
    }
};

#endif
