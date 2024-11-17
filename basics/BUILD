cc_library(
    name = "all",
    srcs = ["basics.cc"],
    hdrs = ["basics.h"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [":all"],
)

cc_test(
    name = "test",
    srcs = ["basics_test.cc"],
    deps = [
        ":all",
        "@googletest//:gtest_main",
    ],
    size = "small",
) 