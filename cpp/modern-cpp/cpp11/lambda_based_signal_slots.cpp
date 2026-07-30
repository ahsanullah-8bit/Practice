/*
    Lambda‑Based Signal‑Slot System

    Features: std::function, lambda expressions, auto, std::vector.
    Goal: A lightweight Signal<Args...> that connect‑s‑slots and calls them when emitted.
    Plan:

    · Define Signal<Args...> holding a std::vector<std::function<void(Args...)>>.
    · Add connect(std::function<void(Args...)>) and emit(Args...). Support lambdas with captures.
    · Test with multiple slots and verify call order.

*/

#include <functional>
#include <stddef.h>
#include <utility>
#include <vector>
#include <iostream>

template<typename ...Args>
class Signal {
public:
    bool connect(std::function<void(Args...)> f) {
        m_slots.push_back(f);
        return true;
    }
    
    bool emit(Args &&...args) {
        for (auto &f : m_slots)
            f(std::forward<Args>(args)...);

        return true;
    }

private:
    std::vector<std::function<void(Args...)>> m_slots;
};

// Test functions for s.
void printDigits(int num_of_digits);
void onAmountChanged(int amount);

// Test functions for s2.
void onNewFootballPlayer(int id, const std::string &name, float score);
void onAddingAGameFriend(int id, const std::string &name, float score);

int main() {
    Signal<int> s;
    s.connect(printDigits);
    s.connect(onAmountChanged);
    s.emit(5);

    std::cout << "\n";
    Signal<int, const std::string&, float> s2; // id, name, score
    s2.connect(onNewFootballPlayer);
    s2.connect(onAddingAGameFriend);
    s2.connect([](int id, const std::string &name, float score) {
        std::cout << "Game Friend (lambda): " << id << ", " << name << ", " << score << std::endl;
    });

    s2.emit(4354, "Michael Jackson", 45.0f);

    return 0;
}

void printDigits(int num_of_digits) {
    std::cout << "Printing Digits: ";
    for (int i = 0; i < num_of_digits; ++i)
        std::cout << i << ", ";
    std::cout << "\n";
}

void onAmountChanged(int amount) {
    std::cout << "New amount: " << amount << std::endl;
}

void onNewFootballPlayer(int id, const std::string &name, float score) {
    // maybe do some database work or whatever...
    std::cout << "Football Player: " << id << ", " << name << ", " << score << std::endl;
}

void onAddingAGameFriend(int id, const std::string &name, float score) {
    // idk
    std::cout << "Game Friend: " << id << ", " << name << ", " << score << std::endl;
}