#include <span>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"

namespace aoc2023 {

class Path {
   public:
    explicit Path(const std::string& start) {
        path_.push_back(start);
    }

    bool CanStep(const std::string& node) {
        return !visited_path_.contains(std::make_pair(node, prev())) &&
               !visited_path_.contains(std::make_pair(prev(), node));
    }

    const std::string& prev() const { return path_.back(); }

    void Step(const std::string& node) {
        visited_path_.insert(std::make_pair(prev(), node));
        visited_path_.insert(std::make_pair(node, prev()));
        path_.push_back(node);
    }

    const absl::flat_hash_set<std::pair<std::string, std::string>>& SeenEdges()
        const {
        return visited_path_;
    }

   private:
    std::vector<std::string> path_;
    absl::flat_hash_set<std::pair<std::string, std::string>> visited_path_;
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
    std::vector<std::string> all_;
};

}  // namespace aoc2023