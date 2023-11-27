cc_binary(
    name = "main",
    srcs = glob(["src/*.c","src/*.h"]),
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
