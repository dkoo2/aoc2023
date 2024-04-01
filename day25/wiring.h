#include <span>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"

namespace aoc2023 {

class Path {
   public:
    explicit Path(const int start) { path_.push_back(start); }

    const int prev() const { return path_.back(); }

    void Step(const int node) {
        path_.push_back(node);
    }
    const std::vector<int>& FullPath() const { return path_; }

   private:
    std::vector<int> path_;
};

class Wiring {
   public:
    explicit Wiring(std::span<const std::string> wirings);
    int size() const { return mapping_fast_.size(); }
    bool Connected() const;

    int MostEncountered();

   private:
    absl::flat_hash_map<int, absl::flat_hash_set<int>> mapping_fast_;
    int all_;
};

}  // namespace aoc2023