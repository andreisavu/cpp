cc_library(
    name = "basics",
    srcs = glob(
        ["*.cc"],
        exclude = [
            "main.cc",
            "*_test.cc",
        ],
    )
    ,
    hdrs = glob(["*.h"]),
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [":basics"],
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
