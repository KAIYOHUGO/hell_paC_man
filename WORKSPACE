load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# http_archive(
#     name = "gtest",
#     sha256 = "f8d7d77c06936315286eb55f8de22cd23c188571",
#     url = "https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip",
# )

http_archive(
  name = "gtest",
  urls = ["https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip"],
  strip_prefix = "googletest-f8d7d77c06936315286eb55f8de22cd23c188571",
)



# # Hedron's Compile Commands Extractor for Bazel
# # https://github.com/hedronvision/bazel-compile-commands-extractor
# http_archive(
#     name = "hedron_compile_commands",

#     # Replace the commit hash in both places (below) with the latest, rather than using the stale one here.
#     # Even better, set up Renovate and let it do the work for you (see "Suggestion: Updates" in the README).
#     url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/ac6411f8f347e5525038cb7858db4969db9e74f2.tar.gz",
#     strip_prefix = "bazel-compile-commands-extractor-ac6411f8f347e5525038cb7858db4969db9e74f2",
#     # When you first run this tool, it'll recommend a sha256 hash to put here with a message like: "DEBUG: Rule 'hedron_compile_commands' indicated that a canonical reproducible form can be obtained by modifying arguments sha256 = ..."
# )

# load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
# hedron_compile_commands_setup()