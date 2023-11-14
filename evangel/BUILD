cc_library(
    name = "evangel",
    srcs = glob(["src/*.c"]),
    hdrs = glob(["src/*.h"]),
    include_prefix = "evangel",
    strip_include_prefix = "/evangel/src",
    visibility = ["//:__pkg__"],
    deps = [":webgpu"],
)

cc_library(
    name = "webgpu",
    hdrs = ["@webgpu//file"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "unit_test",
    srcs = [
        "src/basic.c",
        "src/basic.h",
        "src/test_vec.cpp",
        "src/vec.c",
        "src/vec.h",
    ],
    deps = [
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
