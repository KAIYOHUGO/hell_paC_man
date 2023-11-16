cc_library(
    name = "evangel",
    srcs = glob(["src/*.c"]),
    hdrs = glob(["src/*.h"]),
    include_prefix = "evangel",
    strip_include_prefix = "/evangel/src",
    visibility = ["//visibility:public"],
    deps = ["@wgpu"],
)

# cc_library(
#     name = "wgpu",
#     hdrs = ["@webgpu//file"],
#     # include_prefix = "webgpu",
#     strip_include_prefix = "/external/webgpu/file",
#     visibility = ["//visibility:public"],
# )

cc_test(
    name = "unit_test",
    srcs = [
        "src/basic.c",
        "src/basic.h",
        "src/map.c",
        "src/map.h",
        "src/test.cpp",
        "src/vec.c",
        "src/vec.h",
    ],
    deps = [
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
