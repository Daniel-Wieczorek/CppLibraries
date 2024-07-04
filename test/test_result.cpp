#include "lib/result.hpp"

#include <gtest/gtest.h>

namespace interview
{
namespace library
{
namespace test
{

using namespace interview::library;

class ResultTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ResultTest, DefaultConstructor)
{
    const Result<std::uint32_t> result;
    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ(result.getValue(), 0U);
}

TEST_F(ResultTest, ConstructorWithRValue)
{
    const Result<std::uint32_t> result(42U);
    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ(result.getValue(), 42U);
}

TEST_F(ResultTest, ConstructorWithLValue)
{
    const uint32_t value = 42U;
    Result<std::uint32_t> result(value);
    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ(result.getValue(), value);
}

TEST_F(ResultTest, ConstructorWithLValError)
{
    const Result<std::uint32_t> result(Status::ERROR);
    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(result.getError(), Status::ERROR);
}

TEST_F(ResultTest, ConstructorWithRVAlError)
{
    const Result<std::uint32_t> result(std::move(Status::ERROR));
    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(result.getError(), Status::ERROR);
}

TEST_F(ResultTest, ErrorCreation)
{
    const Result<std::uint32_t> result = createError(Status::INVALID_ARG);
    EXPECT_FALSE(result.hasValue());
    EXPECT_EQ(result.getError(), Status::INVALID_ARG);
}

TEST_F(ResultTest, CopyConstructor)
{
    const Result<std::uint32_t> original(42U);
    const Result<std::uint32_t> copy(original);
    EXPECT_TRUE(copy.hasValue());
    EXPECT_EQ(copy.getValue(), 42U);
}

TEST_F(ResultTest, MoveConstructor)
{
    const Result<std::uint32_t> original(42U);
    const Result<std::uint32_t> moved(std::move(original));
    EXPECT_TRUE(moved.hasValue());
    EXPECT_EQ(moved.getValue(), 42);
}

TEST_F(ResultTest, CopyAssignment)
{
    const Result<std::uint32_t> original(42U);
    const Result<std::uint32_t> copy = original;
    EXPECT_TRUE(copy.hasValue());
    EXPECT_EQ(copy.getValue(), 42U);
}

TEST_F(ResultTest, MoveAssignment)
{
    const Result<std::uint32_t> original(42U);
    const Result<std::uint32_t> moved = std::move(original);
    EXPECT_TRUE(moved.hasValue());
    EXPECT_EQ(moved.getValue(), 42U);
}

TEST_F(ResultTest, GetValue)
{
    const Result<std::uint32_t> result(42U);
    EXPECT_EQ(result.getValue(), 42U);
}

TEST_F(ResultTest, GetValueThrows)
{
    const Result<std::uint32_t> result(Status::ERROR);
    EXPECT_THROW(result.getValue(), std::runtime_error);
}

TEST_F(ResultTest, GetError)
{
    const Result<std::uint32_t> result(Status::INVALID_ARG);
    EXPECT_EQ(result.getError(), Status::INVALID_ARG);
}

TEST_F(ResultTest, GetErrorThrows)
{
    const Result<std::uint32_t> result(42U);
    EXPECT_THROW(result.getError(), std::runtime_error);
}

TEST_F(ResultTest, BooleanConversion)
{
    const Result<std::uint32_t> success(42U);
    const Result<std::uint32_t> failure(Status::ERROR);
    EXPECT_TRUE(static_cast<bool>(success));
    EXPECT_FALSE(static_cast<bool>(failure));
}

TEST_F(ResultTest, HasValue)
{
    const Result<std::uint32_t> success(42U);
    const Result<std::uint32_t> failure(Status::ERROR);
    EXPECT_TRUE(success.hasValue());
    EXPECT_FALSE(failure.hasValue());
}

// Test with a custom type
class CustomType
{
  public:
    CustomType(std::uint32_t data) : data_(data) {}
    const std::uint32_t& getData() const { return data_; }

  private:
    std::uint32_t data_;
};

TEST_F(ResultTest, CustomType)  // Default constructible shall not be call here
{
    const Result<CustomType> result(CustomType(42U));
    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ(result.getValue().getData(), 42U);
}

// Test with a deleted constructor
class NonDefaultConstructible
{
  public:
    NonDefaultConstructible(std::uint32_t data) : data_(data) {}
    NonDefaultConstructible() = delete;
    const std::uint32_t& getData() const { return data_; }

  private:
    std::uint32_t data_;
};

TEST_F(ResultTest, NonDefaultConstructibleType)
{
    const Result<NonDefaultConstructible> result(NonDefaultConstructible(42U));
    EXPECT_TRUE(result.hasValue());
    EXPECT_EQ(result.getValue().getData(), 42U);
}

// Run all the tests
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace test
}  // namespace library
}  // namespace interview
