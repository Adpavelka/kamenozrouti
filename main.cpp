#include "solver.h"
#include "utils.h"

#include <chrono>
#include <iostream>
#include <limits>


int main() {
    Board start = initialBoard();

    std::cout << "Start board:\n";
    printBoard(start);

    Solver solver(BEAM_WIDTH, THREADS);

    auto t0 = std::chrono::high_resolution_clock::now();
    Node best = solver.solve(start);
    auto t1 = std::chrono::high_resolution_clock::now();

    double elapsed = std::chrono::duration<double>(t1 - t0).count();

    std::cout << "Best score found: " << best.score
              << " time: " << elapsed << "s\n";
    best.printing(solver.getArena());

    std::cout << "Press ENTER to exit...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    return 0;
}
