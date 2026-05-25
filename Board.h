#ifndef BOARD_H
#define BOARD_H

#include <array>
#include "config.h"

struct Board {
    std::array<int8_t, H * W> a;
    Board() { a.fill(-1); }

    inline int8_t& at(int r, int c) {
        return a[r * W + c];
    }

    inline int8_t at(int r, int c) const {
        return a[r * W + c]; 
    }

    inline bool isEmpty(int r, int c) const {
        return at(r,c) < 0;
    }
};

#endif
