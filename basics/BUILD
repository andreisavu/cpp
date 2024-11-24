cc_library(
    name = "basics",
    srcs = glob(
        ["*.cc"],
        exclude = [
            "main.cc",
            "*_test.cc",
            "*_benchmark.cc",
        ],
    ),
    hdrs = glob(["*.h"]),
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [":basics"],
)

cc_binary(
    name = "simple_list_benchmark",
    srcs = ["simple_list_benchmark.cc"],
    copts = [
        "-O3",
        "-DNDEBUG",
    ],
    deps = [
        ":basics",
        "@google_benchmark//:benchmark",
    ],
)

cc_test(
    name = "test",
    size = "small",
    srcs = glob(["*_test.cc"]),
    deps = [
        ":basics",
        "@googletest//:gtest_main",
    ],
)
