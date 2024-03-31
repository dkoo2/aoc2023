#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "absl/strings/str_cat.h"
#include "wiring.h"

// CC=clang++ bazel build --enable_bzlmod --action_env=BAZEL_CXXOPTS="-std=c++20" day25:day25

int main() {
    std::ifstream input;
    const std::string filepath = absl::StrCat(
        std::filesystem::current_path().string(), "/", "day25/infile.txt");
    std::cout << "file path : " << filepath << std::endl;
    input.open(filepath);
    assert(input.is_open());
    std::vector<std::string> strings;
    std::string line;
    while (input) {
        getline(input, line);
        strings.push_back(line);
    }
    input.close();

    aoc2023::Wiring wiring(strings);
    // std::cout << wiring.size() << std::endl;
    // std::cout << wiring.Print() << std::endl;
    std::cout << wiring.MostEncountered() << std::endl;
    return 0;
}
