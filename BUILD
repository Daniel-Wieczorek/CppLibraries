
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
    name = "result_lib",
    srcs = ["lib/result.hpp"],
    hdrs = ["lib/result.hpp"],
    copts = safety_warnings,
)

# --- Executables: ---
cc_binary(
    name = "example_result",
    srcs = ["examples/result/example.cpp"],
    copts = safety_warnings,
    deps = [
        ":result_lib",
    ],
)

# --- Tests: ---
cc_test(
    name = "test_result",
    srcs = ["test/test_result.cpp"],
    copts = safety_warnings,
    deps = [
        ":result_lib",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
