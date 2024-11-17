cc_library(
    name = "all",
    srcs = ["basics.cpp"],
    hdrs = ["basics.h"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [":all"],
)

cc_test(
    name = "test",
    srcs = ["basics_test.cpp"],
    deps = [
        ":all",
        "@googletest//:gtest_main",
    ],
    size = "small",
) 