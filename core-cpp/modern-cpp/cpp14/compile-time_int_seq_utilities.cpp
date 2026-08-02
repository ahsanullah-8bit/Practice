/*
	Compile‑Time Integer Sequence Utilities

	Features: std::integer_sequence, std::index_sequence, using aliases.
	Goal: Create make_index_sequence and helpers to apply functions to tuples at compile time.
	Plan:

	· Recap how integer_sequence works.
	· Implement tuple_for_each that uses index sequence to call a lambda on each element.
	· Test with a tuple of mixed types.

	Notes:
		1. Integer Sequences: https://en.cppreference.com/cpp/utility/integer_sequence
		2. Pack: https://en.cppreference.com/cpp/language/pack
		3. Statement folding (discussion): https://groups.google.com/a/isocpp.org/g/std-proposals/c/2hdpLnXJKkQ?pli=1
		4. Partial template specialization: https://en.cppreference.com/cpp/language/partial_specialization
		5. Other approaches: https://codereview.stackexchange.com/questions/51407/stdtuple-foreach-implementation
		6. CRTP: https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
*/


#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>
#include <iostream>

namespace detail {

template<typename T, T I, T N, T... integers>
struct make_integer_sequence_helper {	// [1] [6]
	using type = typename make_integer_sequence_helper<T, I + 1, N, integers..., I>::type;
};

template<typename T, T N, T... integers>
struct make_integer_sequence_helper<T, N, N, integers...> {	// [1] [4]
	using type = std::integer_sequence<T, integers...>;
};

template<typename T, T N>
using make_integer_sequence = typename make_integer_sequence_helper<T, 0, N>::type; // [1]

template<std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>; // [1]

template<typename Tuple, typename Func, std::size_t... I>
void tuple_for_each_impl(Func &&f, Tuple&& t, std::index_sequence<I...>) {
	using expander = int[];
	(void)expander{0, (f(I, std::get<I>(std::forward<Tuple>(t))), 0)...};	// [2] [3] [5]
}

}

template<typename Func, typename Tuple>
void tuple_for_each(Func &&f, Tuple&& t) {
	constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value; // [5] the linked approach's one answer helped with the std::remove_reference_t
	detail::tuple_for_each_impl(std::forward<Func>(f), std::forward<Tuple>(t), detail::make_index_sequence<N>());
}

int main() {
	auto tuple = std::make_tuple("Mike", 3.74f, 25, true);
	tuple_for_each(
		[](std::size_t index, auto &element) {
			std::cout << index << ": " << element << "\n";
		}, tuple);

	return 0;
}