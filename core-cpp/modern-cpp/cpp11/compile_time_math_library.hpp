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

#include <stddef.h>

namespace ctml {

template<typename T>
inline constexpr T factorial(T val) {
    return val <= 1 ? 1 : val * factorial(val - 1);
}

template<typename T>
inline constexpr T fibonacci(T val) {
    return val == 0 ? 0 : 
            (val == 1 ? 1 :
                fibonacci(val - 1) + fibonacci(val - 2));
}

// Uses the Wheel Factorization method https://en.wikipedia.org/wiki/Wheel_factorization
template<typename T>
inline constexpr bool is_prime_helper(T val, size_t i) {
    return i * i > val ? true :
            (val % i == 0 || val % (i + 2) == 0) ? false :
            is_prime_helper(val, i + 6);
}

template<typename T>
inline constexpr bool is_prime(T val) {
    return (val <= 3) ? (val > 1) :
            (val % 2 == 0 || val % 3 == 0) ? false :
            is_prime_helper(val, 5);
}

// Greatest Common Divisor
template<typename T>
inline constexpr T gcd_helper(T a, T b, T g) {
    return g <= 0 ? 1 :
            (a % g == 0 && b % g == 0) ? g :
            gcd_helper(a, b, --g);
}

template<typename T>
inline constexpr T max(T a, T b) {
    return a > b ? a : b;
}

template<typename T>
inline constexpr T min(T a, T b) {
    return a < b ? a : b;
}

template<typename T>
inline constexpr T gcd(T a, T b) {
    return (!a || !b) ? max(a, b) :
            gcd_helper(a, b, min(a, b));
}

}
