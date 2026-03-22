# Game Solver (Beam Search)

This project is a high-performance C++ solver for a grid-based color
matching game (similar to SameGame / Clickomania).\
The solver uses **beam search**, **parallel node expansion**, and
**Zobrist hashing** to efficiently explore large game trees.

------------------------------------------------------------------------

## Features

-   Beam search based solver
-   Multi-threaded node expansion using `std::async`
-   Fast board hashing using Zobrist hashing
-   Heuristic evaluation to guide search
-   Configurable board size, colors, and search parameters
-   Multiple predefined test boards
-   Optional random board generator

------------------------------------------------------------------------

## How It Works

The solver starts from an initial board and repeatedly:

1.  Finds all removable components (groups of 2+ same-colored tiles)
2.  Applies each move to create new boards
3.  Scores the move based on component size
4.  Uses a heuristic estimate to prioritize promising boards
5.  Keeps only the best states using beam search
6.  Repeats until no better moves can be found

------------------------------------------------------------------------

## Project Structure

    main.cpp        - Entry point and solver execution
    solver.cpp      - Beam search implementation
    node.h          - Node representation for the search tree
    board.h         - Board data structure
    component.cpp   - Component extraction and move application
    utils.cpp       - Heuristics, board generation, and utilities
    zobrist.cpp     - Zobrist hashing implementation
    config.h        - Global configuration (board size, beam width, etc.)

------------------------------------------------------------------------

## Configuration

You can change solver parameters in `config.h`:

    static constexpr int H = 10;          // Board height
    static constexpr int W = 20;          // Board width
    static constexpr int COLORS = 5;      // Number of colors
    static constexpr int BEAM_WIDTH = 250000;
    static constexpr int MAX_CHILDREN = 1000000;

------------------------------------------------------------------------

## How to Build

### Using g++

    g++ -O3 -std=c++20 -pthread *.cpp -o solver

------------------------------------------------------------------------

## How to Run

    ./solver

The program will:

1.  Load a predefined board
2.  Run the solver
3.  Print the best score found
4.  Display the sequence of moves

------------------------------------------------------------------------

## Example Output

    Start board:
    ABACD...
    ...

    Best score found: 12000 time: 3.21s

------------------------------------------------------------------------

## Performance Notes

-   The solver is CPU intensive
-   Performance scales with the number of CPU cores
-   Increasing `BEAM_WIDTH` improves solution quality but increases
    runtime
-   The heuristic function estimates future score based on largest
    remaining components

------------------------------------------------------------------------

## Possible Improvements

-   Smarter heuristics
-   GPU acceleration
-   Transposition table optimizations
-   Better pruning strategies
-   GUI visualizer for the solution

------------------------------------------------------------------------

## License

This project is intended for educational and experimentation purposes.
