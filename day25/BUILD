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
        "-flto",
    ],
)

cc_binary(
    name = "day25",
    srcs = ["day25.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        ":wiring",
    ],
    # copts = ["-std=c++20"],
    linkopts = [
        "-flto",
    ],
)