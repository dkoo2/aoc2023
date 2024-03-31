#include "wiring.h"

#include <span>
#include <random>
#include <algorithm>

#include "absl/strings/str_split.h"
#include "absl/strings/str_join.h"
#include "absl/random/random.h"
#include "absl/random/distributions.h"

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
    all_.insert(all_.end(), all_unique.begin(), all_unique.end());
}

int Wiring::MostEncountered() {
    // Select two random nodes and collect the "seen" edges.
    absl::flat_hash_set<std::pair<int, int>> processed;
    const int max_node = all_.size();
    absl::BitGen bitgen;

    absl::flat_hash_map<std::pair<std::string, std::string>, int> edge_counts;
    while (true) {
        if (processed.size() >= 1500) {
            break;
        }
        int from = absl::Uniform(bitgen, 0, max_node);
        int to = absl::Uniform(bitgen, 0, max_node);
        if (from == to || processed.contains(std::make_pair(from, to))) {
            continue;
        }
        processed.insert(std::make_pair(from, to));
        processed.insert(std::make_pair(to, from));
        const std::string& from_str = all_[from];
        const std::string& to_str = all_[to];
        std::vector<Path> queue;
        queue.emplace_back(from_str);
        absl::flat_hash_set<std::pair<std::string, std::string>> global_seen;
        while (!queue.empty()) {
            std::vector<Path> next;
            for (Path& p : queue) {
                if (auto it = mapping_.find(p.prev()); it != mapping_.end()) {
                    for (const std::string& candidate : it->second) {
                        if (candidate == to_str) {
                            assert(p.CanStep(candidate));
                            Path new_p = p;
                            new_p.Step(candidate);
                            for (const auto& edge : new_p.SeenEdges()) {
                                edge_counts[edge]++;
                            }
                        } else if (global_seen.contains(std::make_pair(p.prev(), candidate))) {
                            continue;
                        } else if (p.CanStep(candidate)) {
                            Path new_p = p;
                            new_p.Step(candidate);
                            next.push_back(new_p);
                            global_seen.insert(std::make_pair(p.prev(), candidate));
                            global_seen.insert(std::make_pair(candidate, p.prev()));
                        }
                    }
                }
            }
            queue = std::move(next);
        }
    }
    std::vector<std::tuple<std::string, std::string, int>> sorted;
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
        const std::string& from = std::get<0>(sorted[i]);
        const std::string& to = std::get<1>(sorted[i]);
        mapping_[from].erase(to);
        mapping_[to].erase(from);
    }
    // Pick a random node. Find how many are reachable from this node.
    const std::string& random_node = all_[0];
    absl::flat_hash_set<std::string> seen;
    seen.insert(random_node);
    std::vector<std::string> queue = {random_node};
    while (true) {
        std::vector<std::string> next;
        for (const std::string& el : queue) {
            if (auto it = mapping_.find(el); it != mapping_.end()) {
                for (const std::string& c : it->second) {
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
    absl::flat_hash_set<std::string> visited;
    const std::string& first = mapping_.begin()->first;
    // Add a sink node.
    std::vector<std::string> queue;
    queue.push_back(first);
    visited.insert(first);
    while (visited.size() < all_.size()) {
        std::vector<std::string> next;
        for (const std::string& l : queue) {
            assert(mapping_.contains(l));
            if (auto it = mapping_.find(l); it != mapping_.end()) {
                for (const std::string& tt : it->second) {
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
