cc_library(
    name = "wiring",
    hdrs = ["wiring.h"],
    srcs = ["wiring.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/container:flat_hash_set",
        "@abseil-cpp//absl/random",
    ],
    linkopts = [
        "-lm",
    ],
)

cc_binary(
    name = "day25",
    srcs = ["day25.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        ":wiring",
    ],
    copts = ["--std=c++17", "-O2"],
    linkopts = [
        "-lm",
    ],
)