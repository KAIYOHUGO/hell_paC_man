# run main project (game)
run:
    clear && bazel run //:main --run_under="cd $PWD &&"

# run memory test, output is `valgrind-out.txt`
memtest file:
    valgrind --leak-check=full --track-origins=yes --log-file=valgrind-out.txt bazel-bin/{{file}}

# test all project
test-all:
    bazel test ...

# debug build all project
debug-build:
    bazel build ...  --compilation_mode=dbg -s

# Generate compiles_commands.json for clangd extension, for more info https://github.com/hedronvision/bazel-compile-commands-extractor
gen-compile-commands:
    bazel run @hedron_compile_commands//:refresh_all