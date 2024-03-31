#include "wiring.h"

#include <algorithm>
#include <random>
#include <span>

#include "absl/random/distributions.h"
#include "absl/random/random.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"

namespace aoc2023 {

Wiring::Wiring(const std::vector<std::string>& wirings) {
    absl::flat_hash_set<std::string> all_unique;
    for (const std::string& s : wirings) {
        const std::vector<std::string> split = absl::StrSplit(s, ":");
        assert(split.size() == 2);
        const std::vector<std::string> targets = absl::StrSplit(
            split[1], absl::ByAsciiWhitespace(), absl::SkipEmpty());
        assert(targets.size() >= 1);
        all_unique.insert(split[0]);
        for (const std::string& t : targets) {
            all_unique.insert(t);
            mapping_[split[0]].insert(t);
            mapping_[t].insert(split[0]);
        }
    }
    int index = 0;
    all_.reserve(all_unique.size());
    for (const std::string& str : all_unique) {
        all_.push_back(str);
        name_to_index_[str] = index;
        index++;
    }
    // Rewrite the map using integers.
    for (const auto& [name, neighbors] : mapping_) {
        assert(name_to_index_.contains(name));
        const int index = name_to_index_[name];
        for (const std::string& neighbor : neighbors) {
            mapping_fast_[index].insert(name_to_index_[neighbor]);
        }
    }
}

int Wiring::MostEncountered() {
    // Select two random nodes and collect the "seen" edges.
    absl::flat_hash_set<std::pair<int, int>> processed;
    const int max_node = all_.size();
    absl::BitGen bitgen;

    absl::flat_hash_map<std::pair<int, int>, int> edge_counts;

    auto path_finder_start = std::chrono::system_clock::now();
    while (true) {
        if (processed.size() >= 250) {
            break;
        }
        int from = absl::Uniform(bitgen, 0, max_node);
        int to = absl::Uniform(bitgen, 0, max_node);
        if (from == to || processed.contains(std::make_pair(from, to))) {
            continue;
        }
        processed.insert(std::make_pair(from, to));
        processed.insert(std::make_pair(to, from));

        std::vector<Path> queue;
        queue.emplace_back(from);
        absl::flat_hash_set<std::pair<int, int>> global_seen;
        while (!queue.empty()) {
            std::vector<Path> next;
            for (Path& p : queue) {
                if (auto it = mapping_fast_.find(p.prev());
                    it != mapping_fast_.end()) {
                    for (const int candidate : it->second) {
                        if (candidate == to) {
                            Path new_p = p;
                            new_p.Step(candidate);
                            for (const auto& edge : new_p.SeenEdges()) {
                                edge_counts[edge]++;
                            }
                        } else if (global_seen.contains(
                                       std::make_pair(p.prev(), candidate))) {
                            continue;
                        } else if (p.CanStep(candidate)) {
                            Path new_p = p;
                            new_p.Step(candidate);
                            next.push_back(new_p);
                            global_seen.insert(
                                std::make_pair(p.prev(), candidate));
                            global_seen.insert(
                                std::make_pair(candidate, p.prev()));
                        }
                    }
                }
            }
            queue = std::move(next);
        }
    }
    auto path_finder_end = std::chrono::system_clock::now();
    auto pf_elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(path_finder_end - path_finder_start);
    std::cout << "Path Finder : " << pf_elapsed.count() << std::endl;

    std::vector<std::tuple<int, int, int>> sorted;
    sorted.reserve(edge_counts.size());
    for (const auto& [key, val] : edge_counts) {
        sorted.emplace_back(key.first, key.second, val);
    }
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& lhs, const auto& rhs) {
                  return std::get<2>(lhs) > std::get<2>(rhs);
              });
    // Remove the 6 edges found.
    for (int i = 0; i < 6; ++i) {
        const int from = std::get<0>(sorted[i]);
        const int to = std::get<1>(sorted[i]);
        mapping_fast_[from].erase(to);
        mapping_fast_[to].erase(from);
    }
    // Pick a random node. Find how many are reachable from this node.
    const int random_node = 0;
    absl::flat_hash_set<int> seen;
    seen.insert(random_node);
    std::vector<int> queue = {random_node};
    while (true) {
        std::vector<int> next;
        for (const int el : queue) {
            if (auto it = mapping_fast_.find(el); it != mapping_fast_.end()) {
                for (const int c : it->second) {
                    if (seen.contains(c)) {
                        continue;
                    }
                    next.push_back(c);
                    seen.insert(c);
                }
            }
        }
        if (next.empty()) {
            break;
        }
        queue = std::move(next);
    }
    const int seen_size = seen.size();
    const int unseen_size = all_.size() - seen_size;
    return seen_size * unseen_size;
}

bool Wiring::Connected() const {
    // Pick a node, and BFS outwards, until we can't visit anymore.
    absl::flat_hash_set<int> visited;
    const int first = mapping_fast_.begin()->first;
    // Add a sink node.
    std::vector<int> queue;
    queue.push_back(first);
    visited.insert(first);
    while (visited.size() < all_.size()) {
        std::vector<int> next;
        for (const int l : queue) {
            if (auto it = mapping_fast_.find(l); it != mapping_fast_.end()) {
                for (const int tt : it->second) {
                    if (!visited.contains(tt)) {
                        visited.insert(tt);
                        next.push_back(tt);
                    }
                }
            }
        }
        if (next.empty()) {
            break;
        }
        queue = std::move(next);
    }
    return visited.size() == all_.size();
}

std::string Wiring::Print() const {
    std::string debug;
    for (const auto& [key, targets] : mapping_) {
        absl::StrAppend(&debug, key, " : ");
        for (const std::string& t : targets) {
            absl::StrAppend(&debug, t, " , ");
        }
        absl::StrAppend(&debug, "\n");
    }
    return debug;
}

}  // namespace aoc2023
