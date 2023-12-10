cc_binary(
    name = "main",
    srcs = glob([
        "src/*.c",
        "src/*.h",
    ]),
    deps = [
        "//:miniaudio",
        "//evangel",
    ],
)

cc_library(
    name = "miniaudio",
    hdrs = ["@miniaudio//file"],
    linkopts = [
        "-ldl",
        "-lpthread",
        "-lm",
    ],
    strip_include_prefix = "file/",
    visibility = ["//visibility:public"],
)

cc_test(
    name = "main_test",
    srcs = ["src/test.cpp"],
    deps = [
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
