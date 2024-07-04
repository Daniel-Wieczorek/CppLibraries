load("@com_github_bazelbuild_buildtools//buildifier:def.bzl", "buildifier")

# --- Safety warnings ---
safety_warnings = [
    "-std=c++14",  # Use C++14
    "-Wall",  # Enable all warnings
    "-Wextra",  # Enable extra warnings
    "-Wpedantic",  # Enforce strict C++ standard compliance
    "-Werror",  # Treat warnings as errors
    "-O2",  # Optimization level 2
    "-g",  # Include debug symbols
]

# --- Libraries ---
cc_library(
    name = "result",
    hdrs = ["lib/result.hpp"],
    copts = safety_warnings,
)

# --- Executables: ---
cc_binary(
    name = "interview_app",
    srcs = ["examples/result/example.cpp"],
    copts = safety_warnings,
    deps = [
        ":result",
    ],
)

# --- Tests: ---
cc_test(
    name = "test_result",
    srcs = ["test/test_result.cpp"],
    copts = safety_warnings,
    deps = [
        ":result",
        "@com_google_googletest//:gtest_main",
    ],
)

# --- Other: ---
buildifier(
    name = "buildifier",
)
