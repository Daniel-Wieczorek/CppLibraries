/**
 * @file Result.hpp
 * @brief Definition of the Result class.
 *
 * This file contains the definition of the Result class, which is a generic
 * class that represents the result of an operation. The Result class can hold
 * either a value of type T or an error of type E. It provides methods to access
 * the value or the error, and also provides operators for checking if the
 * result has a value or an error.
 *
 * The Result class is templated on two types: T (the value type) and E (the
 * error type). The error type is optional and defaults to the enum class
 * Status, which represents different status codes.
 *
 * @note This class is part of the custom::library namespace.
 * @author Daniel Wieczorek
 *
 */
#ifndef CUSTOM_LIBRARY_RESULT_HPP
#define CUSTOM_LIBRARY_RESULT_HPP

#include <cstdint>
#include <functional>
#include <iostream>
#include <type_traits>
#include <cstdint>

#if __cplusplus >= 201703L  // C++17 and later
#include <variant>
#endif

namespace custom {
namespace library {

/// @brief Default class as an error type:
enum Status : std::uint32_t { OK = 0, INVALID_ARG, ERROR };

/**
 * @brief Error object creator for `createError`
 *
 * @tparam E type of the error object. `Status` by default and only for now
 */
template <typename E = Status>
class ErrorCreate {
    static_assert(std::is_same<E, Status>::value,
                  "Only Status class is supported as error type now");  // For now only Status class
                                                                        // is supported as Error
   public:
    /// @brief Constructs an Error object with the specified error.
    ErrorCreate(E &&error) noexcept(std::is_nothrow_constructible<E>::value)
        : error_(std::forward<E>(error))
    {
    }

    /// @brief Error getters
    E &getError() & { return error_; }
    const E &getError() const & { return error_; }
    E &&getError() && { return std::move(error_); }
    const E &&getError() const && { return std::move(error_); }

   private:
    E error_;  // error object
};

/// @brief Free function to create an error object, can be called without specifying the type to the
/// template
template <typename E>
ErrorCreate<E> createError(E &&error) noexcept(std::is_nothrow_constructible<E>::value)
{
    return ErrorCreate<E>(std::forward<E>(error));
}

/**
 * @brief A class template representing the result of an operation that can
 * either have a value or an error.
 *
 * @tparam T The type of the value.
 * @tparam E The type of the error. Defaults to `Status`.
 */
template <typename T, typename E = Status>
class Result {
    // Error type must be a Status. Also Status::OK must be 0:
    static_assert(std::is_same<E, Status>::value,
                  "Only Status class is supported as error type now");  // For now only Status class
                                                                        // is supported as Error
    static_assert(
        !std::is_same<T, Status>::value,
        "Status indicates error and can be bind only as an error");  // For now only Status class is
                                                                     // supported as Error
    static_assert(static_cast<int32_t>(Status::OK) == 0, "Status::OK must be 0");

   public:
    /**
     * @brief Default constructor. Constructs a Result object with a
     * default-constructed value.
     *
     * @note Requires `T` to be default constructible.
     */
    template <typename U = T, typename = std::enable_if_t<std::is_default_constructible<U>::value>>
#if __cplusplus >= 201703L
    Result() noexcept(std::is_nothrow_default_constructible<T>::value) : data_(T())
    {
    }
#else
    Result() noexcept(std::is_nothrow_default_constructible<T>::value) : has_value_(true)
    {
        new (&value_) T();
    }
#endif

    /**
     * @brief Constructor for value.
     *
     * @param other universal reference to the value.
     */
    template <typename U = T,
              typename = std::enable_if_t<!std::is_same<std::decay_t<U>, E>::value &&
                                          std::is_constructible<T, U>::value>>
#if __cplusplus >= 201703L
    Result(U &&other) noexcept(std::is_nothrow_constructible<T, U>::value)
        : data_(std::forward<U>(other))
    {
    }
#else
    Result(U &&other) noexcept(std::is_nothrow_constructible<T, U>::value) : has_value_(true)
    {
        new (&value_) T(std::forward<U>(other));
    }
#endif

    /**
     * @brief Constructor for error.
     *
     * @param error universal reference to the error.
     */
    template <typename U = E, typename = std::enable_if_t<std::is_constructible<E, U>::value>>
#if __cplusplus >= 201703L
    Result(E error) noexcept : data_(error)
    {
    }
#else
    Result(E &&error) noexcept(std::is_nothrow_constructible<E, U>::value) : has_value_(false)
    {
        new (&error_) E(std::forward<U>(error));
    }
#endif
    /**
     * @brief Constructs a Result object with an error from an ErrorCreate object.
     *
     * @param error object containing the error value.
     */
    template <typename U>
#if __cplusplus >= 201703L
    Result(ErrorCreate<U> &&error) : data_(std::forward<U>(error.getError())){}
#else
    Result(ErrorCreate<U> &&error) : has_value_(false)
    {
        new (&error_) E(std::forward<E>(error.getError()));
    }
#endif

#if __cplusplus >= 201703L
    // Copy constructor
    Result(const Result &other) noexcept(std::is_nothrow_copy_constructible<T>::value &&
                                         std::is_nothrow_copy_constructible<E>::value) = default;
    // Move constructor
    Result(Result &&other) noexcept(std::is_nothrow_move_constructible<T>::value &&
                                    std::is_nothrow_move_constructible<E>::value) = default;
    // Copy assignment operator
    Result &operator=(const Result &other) noexcept(std::is_nothrow_copy_constructible<T>::value &&
                                                    std::is_nothrow_copy_constructible<E>::value) =
        default;
    // Move assignment operator
    Result &operator=(Result &&other) noexcept(std::is_nothrow_move_constructible<T>::value &&
                                               std::is_nothrow_move_constructible<E>::value) =
        default;
#else
    /**
     * @brief Copy constructor.
     *
     * @param other Result object to copy from.
     */
    Result(const Result &other) noexcept(std::is_nothrow_copy_constructible<T>::value &&
                                         std::is_nothrow_copy_constructible<E>::value)
        : has_value_(other.has_value_)
    {
        if (has_value_) {
            new (&value_) T(other.value_);
        }
        else {
            new (&error_) E(other.error_);
        }
    }

    /**
     * @brief Move constructor.
     *
     * @param other Result object to move from.
     */
    Result(Result &&other) noexcept(std::is_nothrow_move_constructible<T>::value &&
                                    std::is_nothrow_move_constructible<E>::value)
        : has_value_(other.has_value_)
    {
        if (has_value_) {
            new (&value_) T(std::move(other.value_));
        }
        else {
            new (&error_) E(std::move(other.error_));
        }
    }

    /**
     * @brief Copy assignment operator.
     *
     * @param other Result object to copy from.
     * @return reference to the Result object.
     */
    Result &operator=(const Result &other) noexcept(std::is_nothrow_copy_constructible<T>::value &&
                                                    std::is_nothrow_copy_constructible<E>::value)
    {
        if (this != &other) {
            destruct();
            has_value_ = other.has_value();
            if (has_value_) {
                new (&value_) T(other.value_);
            }
            else {
                new (&error_) E(other.error_);
            }
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * @param other Result object to move from.
     * @return reference to the Result object.
     */
    Result &operator=(Result &&other) noexcept(std::is_nothrow_move_constructible<T>::value &&
                                               std::is_nothrow_move_constructible<E>::value)
    {
        if (this != &other) {
            destruct();
            has_value_ = other.has_value();
            if (has_value_) {
                new (&value_) T(std::move(other.value_));
            }
            else {
                new (&error_) E(std::move(other.error_));
            }
        }

        return *this;
    }
#endif

    /**
     * @brief Get the value
     *
     * @return value
     * @throws std::runtime_error If the Result object does not have a value.
     */
    const T &getValue() const &
    {
        if (!hasValue()) {
            throw std::runtime_error("No value");
        }
#if __cplusplus >= 201703L
        return std::get<T>(data_);
#else
        return value_;
#endif
    }

    /**
     * @brief Get the value
     *
     * @return value
     * @throws std::runtime_error If the Result object does not have a value.
     */
    T &getValue() &
    {
        if (!hasValue()) {
            throw std::runtime_error("No value");
        }
#if __cplusplus >= 201703L
        return std::get<T>(data_);
#else
        return value_;
#endif
    }

    /**
     * @brief Get the value
     *
     * @return value
     * @throws std::runtime_error If the Result object does not have a value.
     */
    T &&getValue() &&
    {
        if (!hasValue()) {
            throw std::runtime_error("No value");
        }
#if __cplusplus >= 201703L
        return std::move(std::get<T>(data_));
#else
        return std::move(value_);
#endif
    }

    /**
     * @brief Get the value
     *
     * @return value
     * @throws std::runtime_error If the Result object does not have a value.
     */
    const T &&getValue() const &&
    {
        if (!hasValue()) {
            throw std::runtime_error("No value");
        }
#if __cplusplus >= 201703L
        return std::move(std::get<T>(data_));
#else
        return std::move(value_);
#endif
    }

    /**
     * @brief Get the error
     *
     * @return error.
     * @throws std::runtime_error if the Result object does not have an error.
     */
    E &getError() &
    {
        if (hasValue()) {
            throw std::runtime_error("No error");
        }
#if __cplusplus >= 201703L
        return std::get<E>(data_);
#else
        return error_;
#endif
    }

    /**
     * @brief Get the error
     *
     * @return error.
     * @throws std::runtime_error if the Result object does not have an error.
     */
    const E &getError() const &
    {
        if (hasValue()) {
            throw std::runtime_error("No error");
        }
#if __cplusplus >= 201703L
        return std::get<E>(data_);
#else
        return error_;
#endif
    }

    /**
     * @brief Get the error
     *
     * @return error.
     * @throws std::runtime_error if the Result object does not have an error.
     */
    E &&getError() &&
    {
        if (hasValue()) {
            throw std::runtime_error("No error");
        }
#if __cplusplus >= 201703L
        return std::move(std::get<E>(data_));
#else
        return std::move(error_);
#endif
    }

    /**
     * @brief Get the error
     *
     * @return error.
     * @throws std::runtime_error if the Result object does not have an error.
     */
    const E &&getError() const &&
    {
        if (hasValue()) {
            throw std::runtime_error("No error");
        }
#if __cplusplus >= 201703L
        return std::move(std::get<E>(data_));
#else
        return std::move(error_);
#endif
    }

/// @brief Destructor. Destructs the value or the error.

#if __cplusplus >= 201703L
~Result() = default;
#else
    ~Result() { destruct(); }
#endif
/**
 * @brief Conversion operator to bool.
 *
 * @return `true` if the Result object has a value, `false` otherwise.
 */
explicit operator bool() const noexcept { return hasValue(); }

/**
 * @brief Check if the Result object has a value.
 *
 * @return `true` if the Result object has a value, `false` otherwise.
 */

#if __cplusplus >= 201703L
bool hasValue() const noexcept { return std::holds_alternative<T>(data_); }
#else
    bool hasValue() const noexcept { return has_value_; }
#endif

private:
#if __cplusplus >= 201703L
std::variant<T, E> data_;
#else
    /// @brief Destructs the value or the error.
    void destruct(void) const noexcept(std::is_nothrow_destructible<T>::value)
    {
        if (has_value_) {
            value_.~T();
        }
        else {
            error_.~E();
        }
    }

    union {
        T value_; /* The value. */
        E error_; /* The error. */
    };
    bool has_value_; /* Flag indicating whether the Result object has a value or an error. */
#endif
};

}  // namespace library
}  // namespace custom

#endif  // CUSTOM_LIBRARY_RESULT_HPP