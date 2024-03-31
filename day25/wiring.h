#include <span>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"

namespace aoc2023 {

class Path {
   public:
    explicit Path(const int start) { last_ = start; }

    bool CanStep(const int node) {
        return !visited_path_.contains(std::make_pair(node, prev())) &&
               !visited_path_.contains(std::make_pair(prev(), node));
    }

    const int prev() const { return last_; }

    void Step(const int node) {
        visited_path_.insert(std::make_pair(prev(), node));
        visited_path_.insert(std::make_pair(node, prev()));
        last_ = node;
    }

    const absl::flat_hash_set<std::pair<int, int>>& SeenEdges() const {
        return visited_path_;
    }

   private:
    int last_;
    absl::flat_hash_set<std::pair<int, int>> visited_path_;
};

class Wiring {
   public:
    explicit Wiring(const std::vector<std::string>& wirings);
    int size() const { return mapping_.size(); }
    std::string Print() const;
    bool Connected() const;

    int MostEncountered();

   private:
    absl::flat_hash_map<std::string, absl::flat_hash_set<std::string>> mapping_;
    absl::flat_hash_map<int, absl::flat_hash_set<int>> mapping_fast_;
    absl::flat_hash_map<std::string, int> name_to_index_;
    std::vector<std::string> all_;
};

}  // namespace aoc2023