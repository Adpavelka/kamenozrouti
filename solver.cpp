#include "solver.h"

#include <algorithm>
#include <iostream>

Solver::Solver(int bw, int th)
    : beamWidth(bw), threads(th), Z() {}

std::vector<Node> Solver::expandNode(
    const Node& st,
    std::unordered_map<uint64_t,int>& tt,
    std::mutex& tt_mutex)
{
    std::vector<Node> local;

    auto comps = extractComponents(st.board);
    std::sort(comps.begin(), comps.end(), [](auto &a, auto &b){
        return a.size > b.size;
    });

    int pushes = 0;
    for (auto &comp : comps) {
        if (++pushes > 2000) {
            break;
        }

        Board nb = applyMove(st.board, comp);
        int addScore = getScore(comp.size);
        int newScore = st.score + addScore;
        uint64_t h = Z.hash(nb);

        {
            std::lock_guard<std::mutex> lock(tt_mutex);
            auto it = tt.find(h);
            if (it != tt.end() && it->second >= newScore) {
                continue;
            }
            tt[h] = newScore;
        }

        std::vector<int> newMoves = st.moves;
        if (!comp.cells.empty()) {
            newMoves.push_back(comp.cells.front());
        }

        local.emplace_back(nb, newScore, h, std::move(newMoves));
        local.back().addHeuristic( heuristicEstimate(nb) );

        if ((int)local.size() >= MAX_CHILDREN)
            break;
    }
    return local;
}

void Solver::integrateResults(
    std::vector<Node>& dst,
    std::future<std::vector<Node>>& fut)
{
    auto partial = fut.get();
    for (auto &c : partial) {
        dst.push_back(std::move(c));
    }
}

void Solver::pruneBeam(std::vector<Node>& all) {
    if (all.size() > beamWidth) {
        std::nth_element(all.begin(), all.begin()+beamWidth, all.end(), [](auto&a, auto&b){
            return a.key > b.key;
        });
        all.resize(beamWidth);
    }

    std::sort(all.begin(), all.end(), [](auto&a, auto&b){
        return a.key > b.key;
    });
}

void Solver::updateBest(Node& best, const std::vector<Node>& beam) {
    for (auto &c : beam) {
        if (c.score > best.score) {
            if (best.score / 200 < c.score / 200) {
                best = c;
                best.printing();
            } else {
                best = c;
            }
        }
    }
}

Node Solver::solve(const Board &start) {
    std::unordered_map<uint64_t,int> tt;
    std::mutex tt_mutex;

    Node st0{ start, 0, Z.hash(start), {}};
    std::vector<Node> beam{st0};
    Node best;

    while (!beam.empty()) {
        std::vector<Node> all;
        all.reserve(beam.size() * 16);

        std::vector<std::future<std::vector<Node>>> futures;
        futures.reserve(beam.size());

        // 1. Expand all nodes asynchronously
        for (auto st : beam) {
            futures.push_back(std::async(
                std::launch::async,
                [this, st, &tt, &tt_mutex]() {
                    return expandNode(st, tt, tt_mutex);
                }
            ));
        }

        // 2. Collect results
        for (auto &f : futures) {
            integrateResults(all, f);
        }

        if (all.empty()) {
            break;
        }

        // 3. Prune beam
        pruneBeam(all);

        // 4. Prepare next layer
        beam = all;

        // 5. Update best node
        updateBest(best, beam);
    }

    return best;
}

