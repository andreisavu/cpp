cc_library(
    name = "all",
    srcs = ["main.cpp"],
    hdrs = [],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "main",
    deps = [":all"],
)

cc_test(
    name = "test",
    srcs = ["main_test.cpp"],
    deps = [
        ":all",
        "@com_google_googletest//:gtest_main",
    ],
) 