cc_binary(
    name = "main",
    srcs = ["src/main.c"],
    deps = ["//evangel"],
)

cc_test(
    name = "main_test",
    srcs = ["src/test.cpp"],
    deps = [
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
