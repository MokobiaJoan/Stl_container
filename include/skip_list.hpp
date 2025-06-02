#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <concepts>
#include <iterator>

template <typename T>
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
        head = new Node(T(), maxLevel);
    }

    ~SkipList() {
        Node* current = head;
        while (current) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
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
            while (level > 1 && head->forward[level - 1] == nullptr)
                --level;
        }
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

    iterator begin() const { return iterator(head->forward[0]); }
    iterator end() const { return iterator(nullptr); }

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