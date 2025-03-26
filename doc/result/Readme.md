# Result Class

## Overview

`Result` class is generic class template that represents the result of an
operation. It can hold either a value of type `T` or an error of type `E`. This
class is part of the `custom::library` namespace.

ToDo: Plan for future impl:
`Result` supports `std::variant` introduced in Cpp starting from version 17 and
can be used instead of raw union type.

## Template Parameters

- `T`: type of the value
- `E`: type of the error (default: `Status` enum class)

## Key Features

- Can store either a value or an error
- Provides methods to access the value or error
- Includes boolean operators to check if the result has a value or an error
- Uses union to store either value or error, optimizing memory usage

## Usage Example

```cpp
Result<int> operation(bool succeed) {
    if (succeed) {
        return 42;
    } else {
        return "Operation failed";
    }
}

auto result = operation(true);
if (result) {
    std::cout << "Value: " << result.getValue() << std::endl;
} else {
    std::cout << "Error: " << result.getError() << std::endl;
}
```

## Limitations:
1. Error type should be left for now as default one what is `Status`

## Run targets
To run and test created library you can use `Bazel`

### Run example
To run the the example library usage execute following command:
```Bazel
bazel run //:example_result
```

Example output:

![Image NOT loaded!](img/result_example_output.png)

### Run the tests:
To run the the unit tests execute following command:
```Bazel
bazel run //:test_result
```

Example output:

![Image NOT loaded!](img/result_ut_output.png)