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
    name = "data_structure_test",
    srcs = [
        "src/basic.c",
        "src/basic.h",
        "src/bitset.c",
        "src/bitset.h",
        "src/data_structure_test.cpp",
        "src/map.c",
        "src/map.h",
        "src/vec.c",
        "src/vec.h",
    ],
    deps = [
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "component_test",
    srcs = [
        "src/app.c",
        "src/app.h",
        "src/array.c",
        "src/array.h",
        "src/basic.c",
        "src/basic.h",
        "src/bitset.c",
        "src/bitset.h",
        "src/component.c",
        "src/component.h",
        "src/component_test.cpp",
        "src/event.c",
        "src/event.h",
        "src/map.c",
        "src/map.h",
        "src/vec.c",
        "src/vec.h",
    ],
    deps = [
        "@gtest",
    ],
)
