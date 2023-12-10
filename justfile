run:
    clear && bazel run //:main --run_under="cd $PWD &&"

memtest file:
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind-out.txt bazel-bin/{{file}}

test-all:
    bazel test ...

debug:
    bazel build //:main --compilation_mode=dbg -s

gen-compile-commands:
    bazel run @hedron_compile_commands//:refresh_all