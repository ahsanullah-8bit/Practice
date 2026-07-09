#pragma once

/*
    Compile‑Time Math Library

    Features: constexpr, static_assert, std::integral_constant.
    Goal: A header that provides constexpr factorial, Fibonacci, prime‑check, and GCD.
    Plan:

    · Design the functions as constexpr.
    · Implement iterative constexpr versions (recursion is okay in C++11).
    · Add static_assert tests that validate results at compile time.
*/

#include <algorithm>
#include <stddef.h>

namespace ctml {

template<typename T>
inline constexpr T factorial(T val) {
    if (val <= 1)
        return 1;

    return val * factorial(val - 1);
}

template<typename T>
inline constexpr T fibonacci(T val) {
    if (val == 0)
        return 0;
    if (val == 1)
        return 1;

    return fibonacci(val - 1) + fibonacci(val - 2);
}

// Uses the Wheel Factorization method https://en.wikipedia.org/wiki/Wheel_factorization
template<typename T>
inline constexpr bool is_prime(T val) {
    if (val <= 1) return false;
    if (val <= 3) return true;
    if (val % 2 == 0 || val % 3 == 0) return false;

    for (size_t i = 5; i * i <= val; i += 6) {
        if (val % i == 0 || val % (i + 2) == 0)
            return false;
    }

    return true;
}

// Greatest Common Divisor
template<typename T>
inline constexpr T gcd(T a, T b) {
    if (!a || !b)
        return std::max(a, b);

    for (T i = std::min(a, b); i > 0; --i)
        if (a % i == 0 && b % i == 0)
            return i;

    return 1;
}

}
