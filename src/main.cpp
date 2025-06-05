#include "skip_list.hpp"
#include <iostream>

int main() {
    SkipList<int> sl;
    sl.insert(10);
    sl.insert(5);
    sl.insert(20);

    std::cout << "Contents:\n";
    for (auto v : sl) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    if (sl.find(10)) {
        std::cout << "Found 10\n";
    }

    sl.erase(10);
    std::cout << "After erase:\n";
    for (auto v : sl) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    return 0;
}