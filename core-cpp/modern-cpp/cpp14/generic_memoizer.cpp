/*
    Generic Memoizer

    Features: Generic lambdas, return type deduction, std::unordered_map, std::tuple.
    Goal: A memoize function that takes any callable and returns a caching wrapper.
    Plan:

    · Sketch the wrapper that stores results in a map keyed by argument tuples.
    · Implement using a generic lambda [=](auto... args) and return type deduction. Handle thread safety optionally.
    · Test with an expensive Fibonacci function.

    Notes:
        - Memoization: https://en.wikipedia.org/wiki/Memoization
        - Template argument deduction: https://en.cppreference.com/cpp/language/template_argument_deduction
        - Reference Collapsing: https://en.cppreference.com/cpp/language/reference
                                https://www.ibm.com/docs/en/xl-c-and-cpp-aix/16.1.0?topic=operators-reference-collapsing-c11
        - Mutable: https://en.cppreference.com/cpp/language/cv
*/


#include <map>
#include <tuple>
#include <utility>
#include <iostream>
#include <type_traits>

template<typename F>
auto memoize(F &&f) {
    return [f = std::forward<F>(f)] (auto &self, auto&&... args) mutable {
        using KeyType = std::tuple<std::decay_t<decltype(args)>...>;
        using RetType = typename std::result_of<F(decltype(self), decltype(args)...)>::type;

        static std::map<KeyType, RetType> _cache;

        KeyType key(std::forward<decltype(args)>(args)...);
        auto it = _cache.find(key);
        if (it != _cache.end())
            return it->second;

        RetType result = f(self, std::forward<decltype(args)>(args)...);
        _cache[key] = result;
        return result;
    };
}

int main() {
    auto fib_impl = [] (auto &self, int n) -> int {
        if (n == 0) return 0;
        if (n == 1) return 1;

        return self(self, n - 1) + self(self, n - 2);
    };

    auto memo_fib = memoize(fib_impl);

    std::cout << "Fib: " << memo_fib(memo_fib, 18) << std::endl;

    return 0;
}