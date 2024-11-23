cc_library(
    name = "formats",
    srcs = glob(
        ["*.cc"],
        exclude = [
            "main.cc",
            "*_test.cc",
        ],
    ),
    hdrs = glob(["*.hpp"]),
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [":formats"],
)

cc_test(
    name = "test",
    size = "small",
    srcs = glob(["*_test.cc"]),
    deps = [
        ":formats",
        "@googletest//:gtest_main",
    ],
)
