# Skip List Container

A modern, generic implementation of a Skip List data structure in C++20. It supports STL-like features such as begin(), end(), size(), empty(), operator[], clear(), and swap(). This implementation uses templates and C++20 concepts to ensure type safety and ordering.

# Author
Мокобиа Джоан Чидиебере, Группа 24.Б83_мм

# Contacts
st135508@student.spbu.ru

## Features
- Templated skip list: SkipList<T>
- STL-style forward iterator:
  - begin() and end() for use in range-based loops
- Core operations:
  - insert, erase, find
- Container utilities:
  - size(), empty(), clear(), swap(), operator[]
- Equality operators: ==, !=
- C++20 Concepts:
  - requires std::totally_ordered<T>
- Unit tests with Google Test
- Doxygen documentation with examples and usage
- GitHub Actions CI integration

## Build & Run

### Build Main
make
./build/main

### Run Test
make test

### Doxyfile
doxygen docs/Doxyfile
### Run
xdg-open docs/html/index.html