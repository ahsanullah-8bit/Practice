#include <cstddef>
#include <iostream>
#include <string>
#include <tuple>

//tuple, get, istringstream, auto

// Base
template<typename T>
T from_string(const std::string &str);

// std::string specialization
template<>
std::string from_string(const std::string &str) {
    return str;
}

// int specialization
template<>
int from_string(const std::string &str) {
    return std::stoi(str);
}

// float specialization
template<>
float from_string(const std::string &str) {
    return std::stof(str);
}

// double specialization
template<>
double from_string (const std::string &str) {
    return std::stod(str);
}

template<>
char from_string(const std::string &str) {
    return str.at(0);
}

// Base case
template<typename Last>
std::tuple<Last> parse_csv(std::string line) {
    return std::make_tuple(from_string<Last>(line));
}

template<typename First, typename Second, typename... Args>
std::tuple<First, Second, Args...> parse_csv(std::string line) {
    size_t comma = line.find(',');
    std::string val = line.substr(0, comma);
    std::string left_string = (comma == std::string::npos) ? "" : line.substr(comma + 1);

    auto first = from_string<First>(val);

    return std::tuple_cat(std::make_tuple(first), parse_csv<Second, Args...>(left_string));
}

int main() {

    std::string line = "123,43.5,abc";
    auto values = parse_csv<int, double, std::string>(line);

    std::cout << "int: " << std::get<0>(values) << " + 5 = " << std::get<0>(values) + 5 << std::endl;
    std::cout << "double: " << std::get<1>(values) << std::endl;
    std::cout << "string: " << std::get<2>(values) << std::endl;

    return 0;
}