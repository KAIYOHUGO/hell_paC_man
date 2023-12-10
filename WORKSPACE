load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive", "http_file")

http_archive(
    name = "gtest",
    strip_prefix = "googletest-f8d7d77c06936315286eb55f8de22cd23c188571",
    urls = ["https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip"],
)

http_archive(
    name = "wgpu",
    build_file = "@//:wgpu.BUILD",
    urls = ["https://github.com/gfx-rs/wgpu-native/releases/download/v0.18.0.2/wgpu-linux-x86_64-release.zip"],
)

http_file(
    name = "miniaudio",
    downloaded_file_path = "miniaudio.h",
    sha256 = "6b2029714f8634c4d7c70cc042f45074e0565766113fc064f20cd27c986be9c9",
    url = "https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h",
)

# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
http_archive(
    name = "hedron_compile_commands",
    strip_prefix = "bazel-compile-commands-extractor-ac6411f8f347e5525038cb7858db4969db9e74f2",

    # Replace the commit hash in both places (below) with the latest, rather than using the stale one here.
    # Even better, set up Renovate and let it do the work for you (see "Suggestion: Updates" in the README).
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/ac6411f8f347e5525038cb7858db4969db9e74f2.tar.gz",
    # When you first run this tool, it'll recommend a sha256 hash to put here with a message like: "DEBUG: Rule 'hedron_compile_commands' indicated that a canonical reproducible form can be obtained by modifying arguments sha256 = ..."
)

load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")

hedron_compile_commands_setup()
