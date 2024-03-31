#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "absl/strings/str_cat.h"
#include "wiring.h"

// CC=clang++ bazel build --enable_bzlmod --action_env=BAZEL_CXXOPTS="-std=c++20" day25:day25
// CC=clang++ bazel build --enable_bzlmod day25:day25
int main() {
    std::ifstream input;
    const std::string filepath = absl::StrCat(
        std::filesystem::current_path().string(), "/", "day25/infile.txt");
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
    int most = -1;

    auto start = std::chrono::system_clock::now();
    most = wiring.MostEncountered();
    auto end = std::chrono::system_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Encountered : " << most << " " << elapsed.count() << std::endl;

    return 0;
}
