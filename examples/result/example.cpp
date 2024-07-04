#include "lib/result.hpp"

#include <iostream>
#include <string>
#include <vector>

interview::library::Result<std::uint32_t> divideNumbers(std::uint32_t a, std::uint32_t b)
{
    if (b == 0)
    {
        return interview::library::createError(interview::library::Status::INVALID_ARG);
    }
    return static_cast<std::uint32_t>(a / b);
}

interview::library::Result<std::string> greetName(const std::string& name)
{
    if (name.empty())
    {
        return interview::library::createError(interview::library::Status::INVALID_ARG);
    }
    return std::string("Hello, " + name + "!");
}

interview::library::Result<std::uint32_t> readData(const std::vector<std::uint32_t>& data)
{
    if (data.empty())
    {
        return interview::library::createError(interview::library::Status::INVALID_ARG);
    }
    return data[0];
}

interview::library::Result<std::string> stringParseWithError(const bool error)
{
    if (error)
    {
        return interview::library::createError(interview::library::Status::ERROR);
    }
    return std::string("Parsed string");
}

int32_t main()
{

    const auto result1 = divideNumbers(10U, 2U);
    if (result1)
    {
        std::cout << "Result of 10 / 2: " << result1.getValue() << std::endl;
    }
    else
    {
        std::cout << "divideNumbers error: " << static_cast<std::uint32_t>(result1.getError()) << std::endl;
    }

    const auto result2 = divideNumbers(10U, 0U);
    if (result2)
    {
        std::cout << "Result of 10 / 0: " << result2.getValue() << std::endl;
    }
    else
    {
        std::cout << "divideNumbers error: " << static_cast<std::uint32_t>(result2.getError()) << std::endl;
    }

    const auto result3 = greetName("Daniel");
    if (result3)
    {
        std::cout << result3.getValue() << std::endl;
    }
    else
    {
        std::cout << "greetName error status: " << static_cast<std::uint32_t>(result3.getError()) << std::endl;
    }

    const auto result4 = greetName("");
    if (result4)
    {
        std::cout << result4.getValue() << std::endl;
    }
    else
    {
        std::cout << "greetName error status: " << static_cast<std::uint32_t>(result4.getError()) << std::endl;
    }

    // Example 3: Move semantics
    const interview::library::Result<std::string> result5 = "Hello, World!";
    const interview::library::Result<std::string> result6 = std::move(result5);

    if (result6)
    {
        std::cout << "Moved result: " << result6.getValue() << std::endl;
    }

    std::vector<std::uint32_t> data = {42U, 43U, 44U};
    auto result7 = readData(data);

    if (result7)
    {
        std::cout << "readData Read from vector: " << result7.getValue() << std::endl;
    }
    else
    {
        std::cout << "readData error status: " << static_cast<std::uint32_t>(result7.getError()) << std::endl;
    }

    data.clear();
    const auto result8 = readData(data);

    if (result8)
    {
        std::cout << "readData Read from empty vector: " << result8.getValue() << std::endl;
    }
    else
    {
        std::cout << "readData error status: " << static_cast<std::uint32_t>(result8.getError()) << std::endl;
    }

    const auto result9 = stringParseWithError(true);
    if (result9)
    {
        std::cout << "stringParseWithError success " << result9.getValue() << std::endl;
    }
    else
    {
        std::cout << "stringParseWithError error status: " << static_cast<std::uint32_t>(result9.getError())
                  << std::endl;
    }

    return 0;
}