cc_library(
    name = "all",
    srcs = glob(
        ["*.cc"],
        exclude = [
            "main.cc",
            "*_test.cc",
        ],
    ),
    hdrs = glob(["*.h"]),
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [":all"],
)

cc_test(
    name = "test",
    size = "small",
    srcs = glob(["*_test.cc"]),
    deps = [
        ":all",
        "@googletest//:gtest_main",
    ],
)
