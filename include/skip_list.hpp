#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <concepts>
#include <iterator>
#include <stdexcept>
#include <algorithm>

template <typename T>
requires requires (T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
}
class SkipList {
private:
    struct Node {
        T value;
        std::vector<Node*> forward;
        Node(const T& val, int level) : value(val), forward(level, nullptr) {}
    };

    static constexpr float probability = 0.5;
    static constexpr int maxLevel = 16;

    Node* head;
    int level;
    int count;

    int randomLevel() {
        int lvl = 1;
        while ((std::rand() / double(RAND_MAX)) < probability && lvl < maxLevel)
            ++lvl;
        return lvl;
    }

public:
    SkipList() {
        std::srand(std::time(nullptr));
        level = 1;
        count = 0;
        head = new Node(T(), maxLevel);
    }

    ~SkipList() {
        clear();
        delete head;
    }

    void insert(const T& value) {
        std::vector<Node*> update(maxLevel, nullptr);
        Node* current = head;

        for (int i = level - 1; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->value < value)
                current = current->forward[i];
            update[i] = current;
        }

        current = current->forward[0];
        if (!current || current->value != value) {
            int newLevel = randomLevel();
            if (newLevel > level) {
                for (int i = level; i < newLevel; ++i)
                    update[i] = head;
                level = newLevel;
            }

            Node* newNode = new Node(value, newLevel);
            for (int i = 0; i < newLevel; ++i) {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
            ++count;
        }
    }

    bool find(const T& value) const {
        Node* current = head;
        for (int i = level - 1; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->value < value)
                current = current->forward[i];
        }
        current = current->forward[0];
        return current && current->value == value;
    }

    void erase(const T& value) {
        std::vector<Node*> update(maxLevel, nullptr);
        Node* current = head;

        for (int i = level - 1; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->value < value)
                current = current->forward[i];
            update[i] = current;
        }

        current = current->forward[0];
        if (current && current->value == value) {
            for (int i = 0; i < level; ++i) {
                if (update[i]->forward[i] != current)
                    break;
                update[i]->forward[i] = current->forward[i];
            }
            delete current;
            --count;
            while (level > 1 && head->forward[level - 1] == nullptr)
                --level;
        }
    }

    void clear() {
        Node* current = head->forward[0];
        while (current) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
        std::fill(head->forward.begin(), head->forward.end(), nullptr);
        level = 1;
        count = 0;
    }

    int size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }

    void swap(SkipList& other) {
        std::swap(head, other.head);
        std::swap(level, other.level);
        std::swap(count, other.count);
    }

    T& operator[](int index) {
        if (index < 0 || index >= count)
            throw std::out_of_range("Index out of bounds");

        Node* current = head->forward[0];
        for (int i = 0; i < index; ++i)
            current = current->forward[0];
        return current->value;
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= count)
            throw std::out_of_range("Index out of bounds");

        Node* current = head->forward[0];
        for (int i = 0; i < index; ++i)
        current = current->forward[0];
        return current->value;
    }

    // Iterator
    class iterator {
        Node* current;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(Node* node) : current(node) {}
        T& operator*() const { return current->value; }
        iterator& operator++() {
            current = current->forward[0];
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        bool operator==(const iterator& other) const {
            return current == other.current;
        }
        bool operator!=(const iterator& other) const {
            return current != other.current;
        }
    };

    class const_iterator {
        const Node* current;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator(const Node* node) : current(node) {}
        const T& operator*() const { return current->value; }
        const_iterator& operator++() {
            current = current->forward[0];
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        bool operator==(const const_iterator& other) const {
            return current == other.current;
        }
        bool operator!=(const const_iterator& other) const {
            return current != other.current;
        }
    };

    iterator begin() { return iterator(head->forward[0]); }
    iterator end() { return iterator(nullptr); }
    const_iterator begin() const { return const_iterator(head->forward[0]); }
    const_iterator end() const { return const_iterator(nullptr); }

    bool operator==(const SkipList<T>& other) const {
        auto it1 = begin(), it2 = other.begin();
        for (; it1 != end() && it2 != other.end(); ++it1, ++it2) {
            if (*it1 != *it2) return false;
        }
        return it1 == end() && it2 == other.end();
    }

    bool operator!=(const SkipList<T>& other) const {
        return !(*this == other);
    }
};