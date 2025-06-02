#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <concepts>
#include <iterator>

/**
 * @brief Templated Skip List class that supports fast insert, search, and erase operations.
 * 
 * @tparam T Type of the elements stored (must support comparison operators).
 */

template <typename T>
class SkipList {
private:

 /**
     * @brief Node structure used internally by SkipList.
     */
    struct Node {
        T value;
        std::vector<Node*> forward;
        Node(const T& val, int level) : value(val), forward(level, nullptr) {}
    };

      /**
         * @brief Constructs a new node with a given value and level.
         * @param val Value to store in the node.
         * @param level Number of forward pointers (levels).
         */

    static constexpr float probability = 0.5;
    static constexpr int maxLevel = 16;
    Node* head;
    int level;

     /**
     * @brief Randomly generate a level for a new node.
     * @return Level number (0-based).
     */

    int randomLevel() {
        int lvl = 1;
        while ((std::rand() / double(RAND_MAX)) < probability && lvl < maxLevel)
            ++lvl;
        return lvl;
    }

public:
   /**
     * @brief Constructs an empty SkipList.
     * @param maxLevel Maximum height of the skip list.
     * @param probability Probability used to determine node levels.
     */
    SkipList() {
        std::srand(std::time(nullptr));
        level = 1;
        head = new Node(T(), maxLevel);
    }

     /**
     * @brief Destructor. Frees all allocated memory.
     */

    ~SkipList() {
        Node* current = head;
        while (current) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
    }

     /**
     * @brief Inserts a value into the skip list.
     * 
     * @param value The value to insert.
     * 
     * @note Duplicate values are allowed.
     * 
     */

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

       /**
     * @brief Erases a value from the skip list.
     * @param value The value to remove.
     */

    bool find(const T& value) const {
        Node* current = head;
        for (int i = level - 1; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->value < value)
                current = current->forward[i];
        }
        current = current->forward[0];
        return current && current->value == value;
    }

    /**
     * @brief Removes a value from the skip list if it exists.
     * 
     * @param value The value to remove.
     * 
     * @example
     * SkipList<int> sl;
     * sl.insert(10);
     * sl.erase(10);
     */

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

       /**
     * @brief Finds whether a value exists in the skip list.
     * @param value The value to find.
     * @return true if found, false otherwise.
     */

 // Iterator

     /**
     * @brief Forward iterator for SkipList (STL-style).
     * 
     * @example
     * SkipList<int> sl;
     * sl.insert(1);
     * sl.insert(2);
     * for (auto it = sl.begin(); it != sl.end(); ++it) {
     *     std::cout << *it << " ";
     * }
     */

    class iterator {
        Node* current;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

         /**
         * @brief Constructs an iterator from a node pointer.
         * @param node The node to start from.
         */

        iterator(Node* node) : current(node) {}

         /**
         * @brief Dereference operator.
         * @return Reference to the value at current node.
         */

        T& operator*() const { return current->value; }

         /**
         * @brief Pre-increment operator.
         * @return Reference to the updated iterator.
         */

        iterator& operator++() {
            current = current->forward[0];
            return *this;
        }

         /**
         * @brief Post-increment operator.
         * @return Copy of iterator before increment.
         */

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

         /**
         * @brief Equality operator.
         * @param other Another iterator.
         * @return true if both point to the same node.
         */

        bool operator==(const iterator& other) const {
            return current == other.current;
        }

         /**
         * @brief Inequality operator.
         * @param other Another iterator.
         * @return true if they point to different nodes.
         */

        bool operator!=(const iterator& other) const {
            return current != other.current;
        }
    };

      /**
     * @brief Returns an iterator to the beginning of the list.
     * @return Iterator to first element.
     */

    iterator begin() const { return iterator(head->forward[0]); }

      /**
     * @brief Returns an iterator to the end (nullptr).
     * @return Iterator to end.
     */

    iterator end() const { return iterator(nullptr); }

     /**
     * @brief Checks if two SkipLists are equal (contain same elements in same order).
     * 
     * @param other The other skip list to compare.
     * @return true if equal.
     * 
     * @example
     * SkipList<int> a, b;
     * a.insert(1); b.insert(1);
     * bool same = (a == b); // true
     */

    bool operator==(const SkipList<T>& other) const {
        auto it1 = begin(), it2 = other.begin();
        for (; it1 != end() && it2 != other.end(); ++it1, ++it2) {
            if (*it1 != *it2) return false;
        }
        return it1 == end() && it2 == other.end();
    }

    /**
     * @brief Checks if two SkipLists are not equal.
     * 
     * @param other The other skip list.
     * @return true if not equal.
     */

    bool operator!=(const SkipList<T>& other) const {
        return !(*this == other);
    }
};