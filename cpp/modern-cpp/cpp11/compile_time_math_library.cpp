/*
    Compile‑Time Math Library

    Features: constexpr, static_assert, std::integral_constant.
    Goal: A header that provides constexpr factorial, Fibonacci, prime‑check, and GCD.
    Plan:

    · Design the functions as constexpr.
    · Implement iterative constexpr versions (recursion is okay in C++11).
    · Add static_assert tests that validate results at compile time.
*/

#include "compile_time_math_library.hpp"
#include <assert.h>

int main() {
    static_assert(ctml::factorial<long long>(19) == 121645100408832000LL);
    static_assert(ctml::fibonacci(10) == 55);
    static_assert(ctml::is_prime(11) == true);
    static_assert(ctml::gcd(20, 28) == 4);

    return 0;
}