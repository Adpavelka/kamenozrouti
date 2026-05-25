#include "solver.h"

#include <algorithm>
#include <iostream>

Solver::Solver(int bw, int th)
    : beamWidth(bw), pool(th), Z() {}

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

        int newMoveIdx = -1;
        if (!comp.cells.empty()) {
            newMoveIdx = arena.add(comp.cells.front(), st.moveIdx);
        }

        local.emplace_back(nb, newScore, h, newMoveIdx);
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
            best = c;
            if (best.score % 200 < c.score % 200) {
                best.printing(arena);
            }
        }
    }
}

Node Solver::solve(const Board &start) {
    std::unordered_map<uint64_t, int> tt;
    tt.reserve(beamWidth * 2);
    std::mutex tt_mutex;

    Node st0{ start, 0, Z.hash(start), -1};
    std::vector<Node> beam{st0};
    Node best;

    while (!beam.empty()) {
        tt.clear();
        tt.reserve(beamWidth * 2);

        std::vector<Node> all;
        all.reserve(std::min((int)beam.size() * 16, beamWidth * 8));

        std::vector<std::future<std::vector<Node>>> futures;
        int numThreads = pool.size();
        int chunkSize  = std::max(1, (int)beam.size() / numThreads);

        for (int t = 0; t < numThreads; ++t) {
            int lo = t * chunkSize;
            int hi = (t == numThreads - 1)
                     ? (int)beam.size()
                     : std::min(lo + chunkSize, (int)beam.size());
            if (lo >= (int)beam.size()) break;

            // pool.enqueue instead of std::async
            futures.push_back(pool.enqueue(
                [this, lo, hi, &beam, &tt, &tt_mutex]() {
                    std::vector<Node> local;
                    for (int i = lo; i < hi; ++i) {
                        auto children = expandNode(beam[i], tt, tt_mutex);
                        for (auto& c : children)
                            local.push_back(std::move(c));
                    }
                    return local;
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

