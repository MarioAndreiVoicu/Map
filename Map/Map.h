#pragma once
#include "RBT.h"
#include <stdexcept>

/**
 * @brief Map implementation using a red–black tree.
 *
 * This class wraps an RBT to provide a map interface
 * with key-value storage and iterator support.
 *
 * @tparam KeyType   The type of keys.
 * @tparam ValueType The type of values.
 * @tparam Compare   The comparator type (default: std::less<KeyType>).
 */
template <typename KeyType, typename ValueType, typename Compare = std::less<KeyType>>
class Map
{
    RBT<KeyType, ValueType, Compare> tree;
    Compare comparator;

public:
    using KeyValue = std::pair<KeyType, ValueType>;

    class Iterator;
    class ConstIterator;

    // Constructors and destructor
    Map(const Compare& comparator = Compare());
    Map(const Map& other, const Compare& comparator = Compare());
    Map(Iterator itBegin, Iterator itEnd, const Compare& comparator = Compare());
    Map& operator=(const Map& other);
    ~Map() {}

    // Basic member functions
    typename RBT<KeyType, ValueType, Compare>::Node* find(const KeyType& key);
    ValueType& operator[](const KeyType& key);
    void insert(const KeyValue& KeyValue);
    bool empty();
    int size();
    int count(const KeyType& key);
    KeyValue& max();
    KeyValue& min();
    void erase(const KeyType& keyDeleted);
    void clear();
    void display();

    // Comparison and merging
    bool operator==(Map<KeyType, ValueType, Compare>& other);
    bool operator!=(const RBT<KeyType, ValueType, Compare>& other);

    /**
     * @brief Merges another map into this one.
     *
     * Inserts all key-value pairs from mergeMap into this map.
     *
     * @param mergeMap The map to merge.
     */
    void mergeMaps(Map<KeyType, ValueType, Compare>& mergeMap);

    // Iterator functions
    Iterator begin();
    Iterator end();
    Iterator lower_bound(const KeyType& key);
    Iterator upper_bound(const KeyType& key);

    ConstIterator begin() const;
    ConstIterator end() const;
    ConstIterator cbegin() const;
    ConstIterator cend() const;

    // Iterator classes
    /**
     * @brief Iterator for Map.
     */
    class Iterator
    {
        typename RBT<KeyType, ValueType, Compare>::Node* node;
        Map* map;
    public:
        Iterator(typename RBT<KeyType, ValueType, Compare>::Node* nodePointer, Map* mapPointer)
            : node(nodePointer), map(mapPointer) {}

        Iterator& operator++();
        Iterator operator++(int);
        Iterator& operator--();
        Iterator operator--(int);
        Iterator operator+(int n);
        Iterator operator-(int n);
        Iterator& operator+=(int n);
        Iterator& operator-=(int n);
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
        KeyValue& operator*();
        KeyValue* operator->();
        Iterator& operator=(const Iterator& other);
        Iterator& prev();
        Iterator& next();

        friend class ConstIterator;
    };

    /**
     * @brief ConstIterator for Map.
     */
    class ConstIterator
    {
        const typename RBT<KeyType, ValueType, Compare>::Node* node;
        const Map* map;
    public:
        ConstIterator(const typename RBT<KeyType, ValueType, Compare>::Node* nodePointer, const Map* mapPointer)
            : node(nodePointer), map(mapPointer) {}

        ConstIterator& operator++();
        ConstIterator operator++(int);
        ConstIterator& operator--();
        ConstIterator operator--(int);
        ConstIterator operator+(int n);
        ConstIterator operator-(int n);
        ConstIterator& operator+=(int n);
        ConstIterator& operator-=(int n);
        bool operator==(const ConstIterator& other) const;
        bool operator!=(const ConstIterator& other) const;
        const KeyValue& operator*() const;
        const KeyValue* operator->() const;
        ConstIterator& operator=(const ConstIterator& other);
        ConstIterator& prev();
        ConstIterator& next();

        friend class Iterator;
    };
};

/* ======================= Map Member Function Implementations ======================= */

template<typename KeyType, typename ValueType, typename Compare>
Map<KeyType, ValueType, Compare>::Map(const Compare& comparator)
    : tree(comparator), comparator(comparator)
{}

template<typename KeyType, typename ValueType, typename Compare>
Map<KeyType, ValueType, Compare>::Map(const Map& other, const Compare& comparator)
    : tree(other.tree, comparator), comparator(comparator)
{}

template<typename KeyType, typename ValueType, typename Compare>
Map<KeyType, ValueType, Compare>::Map(Iterator itBegin, Iterator itEnd, const Compare& comparator)
    : tree(comparator), comparator(comparator)
{
    for (Iterator it = itBegin; it != itEnd; ++it)
        tree.insert(*it);
}

template<typename KeyType, typename ValueType, typename Compare>
typename RBT<KeyType, ValueType, Compare>::Node* Map<KeyType, ValueType, Compare>::find(const KeyType& key)
{
    return tree.find(key);
}

template<typename KeyType, typename ValueType, typename Compare>
ValueType& Map<KeyType, ValueType, Compare>::operator[](const KeyType& key)
{
    return tree[key];
}

template<typename KeyType, typename ValueType, typename Compare>
void Map<KeyType, ValueType, Compare>::insert(const KeyValue& KeyValue)
{
    tree.insert(KeyValue);
}

template<typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::empty()
{
    return tree.empty();
}

template<typename KeyType, typename ValueType, typename Compare>
int Map<KeyType, ValueType, Compare>::size()
{
    return tree.size();
}

template<typename KeyType, typename ValueType, typename Compare>
int Map<KeyType, ValueType, Compare>::count(const KeyType& key)
{
    return tree.find(key) ? 1 : 0;
}

template<typename KeyType, typename ValueType, typename Compare>
std::pair<KeyType, ValueType>& Map<KeyType, ValueType, Compare>::max()
{
    return tree.max(tree.getRoot())->KeyValuePair;
}

template<typename KeyType, typename ValueType, typename Compare>
std::pair<KeyType, ValueType>& Map<KeyType, ValueType, Compare>::min()
{
    return tree.min(tree.getRoot())->KeyValuePair;
}

template<typename KeyType, typename ValueType, typename Compare>
void Map<KeyType, ValueType, Compare>::erase(const KeyType& keyDeleted)
{
    tree.deleteKey(keyDeleted);
}

template<typename KeyType, typename ValueType, typename Compare>
void Map<KeyType, ValueType, Compare>::clear()
{
    tree.clear();
}

template<typename KeyType, typename ValueType, typename Compare>
void Map<KeyType, ValueType, Compare>::display()
{
    tree.displayKeyValue();
}

template<typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::operator==(Map<KeyType, ValueType, Compare>& other)
{
    if (tree.size() != other.tree.size())
        return false;

    Iterator it1 = begin();
    Iterator it2 = other.begin();
    while (it1 != end())
    {
        if (it1 != it2)
            return false;
        ++it1;
        ++it2;
    }
    return true;
}

template<typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::operator!=(const RBT<KeyType, ValueType, Compare>& other)
{
    return !(tree == other);
}

template<typename KeyType, typename ValueType, typename Compare>
void Map<KeyType, ValueType, Compare>::mergeMaps(Map<KeyType, ValueType, Compare>& mergeMap)
{
    for (Iterator it = mergeMap.begin(); it != mergeMap.end(); ++it)
    {
        std::pair<KeyType, ValueType> pair = *it;
        tree.insert(pair);
    }
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::begin()
{
    return Iterator(tree.min(tree.getRoot()), this);
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::end()
{
    return Iterator(tree.getSentinel(), this);
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator Map<KeyType, ValueType, Compare>::begin() const
{
    return ConstIterator(tree.min(tree.getRoot()), this);
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator Map<KeyType, ValueType, Compare>::end() const
{
    return ConstIterator(tree.getSentinel(), this);
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator Map<KeyType, ValueType, Compare>::cbegin() const
{
    return begin();
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator Map<KeyType, ValueType, Compare>::cend() const
{
    return end();
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::lower_bound(const KeyType& key)
{
    typename RBT<KeyType, ValueType, Compare>::Node* lowerNode = nullptr;
    typename RBT<KeyType, ValueType, Compare>::Node* node = tree.getRoot();
    while (node != tree.getSentinel())
    {
        if (node->KeyValuePair.first == key)
            return Iterator(node, this);
        if (!comparator(key, node->KeyValuePair.first))
        {
            lowerNode = node;
            node = node->left;
        }
        else
            node = node->right;
    }
    return (lowerNode == nullptr) ? end() : Iterator(lowerNode, this);
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::upper_bound(const KeyType& key)
{
    typename RBT<KeyType, ValueType, Compare>::Node* upperNode = nullptr;
    typename RBT<KeyType, ValueType, Compare>::Node* node = tree.getRoot();
    while (node != tree.getSentinel())
    {
        if (comparator(key, node->KeyValuePair.first))
        {
            upperNode = node;
            node = node->left;
        }
        else
            node = node->right;
    }
    return (upperNode == nullptr) ? end() : Iterator(upperNode, this);
}

/* ======================= Iterator Implementations ======================= */

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator& Map<KeyType, ValueType, Compare>::Iterator::operator++()
{
    if (*this == map->end())
        return *this;
    auto successorNode = map->tree.successor(node);
    node = successorNode ? successorNode : map->tree.getSentinel();
    return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::Iterator::operator++(int)
{
    Iterator temp = *this;
    ++(*this);
    return temp;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator& Map<KeyType, ValueType, Compare>::Iterator::operator--()
{
    if (*this == Iterator(map->tree.min(map->tree.getRoot()), map))
        throw std::out_of_range("Cannot decrement iterator past the beginning.");
    if (*this == map->end())
    {
        node = map->tree.max(map->tree.getRoot());
        return *this;
    }
    auto predecessorNode = map->tree.predecessor(node);
    if (!predecessorNode)
        throw std::out_of_range("Cannot decrement iterator past the beginning.");
    node = predecessorNode;
    return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::Iterator::operator--(int)
{
    Iterator temp = *this;
    --(*this);
    return temp;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::Iterator::operator+(int n)
{
    Iterator temp = *this;
    for (int i = 0; i < n; i++)
        ++temp;
    return temp;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::Iterator::operator-(int n)
{
    Iterator temp = *this;
    for (int i = 0; i < n; i++)
        --temp;
    return temp;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator& Map<KeyType, ValueType, Compare>::Iterator::operator+=(int n)
{
    if (n < 0)
    {
        while (n < 0)
        {
            ++n;
            --(*this);
        }
    }
    else
    {
        while (n > 0)
        {
            --n;
            ++(*this);
        }
    }
    return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator& Map<KeyType, ValueType, Compare>::Iterator::operator-=(int n)
{
    if (n < 0)
    {
        while (n < 0)
        {
            ++n;
            ++(*this);
        }
    }
    else
    {
        while (n > 0)
        {
            --n;
            --(*this);
        }
    }
    return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::Iterator::operator==(const Iterator& other) const
{
    return node == other.node && map == other.map;
}

template<typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::Iterator::operator!=(const Iterator& other) const
{
    return !(*this == other);
}

template<typename KeyType, typename ValueType, typename Compare>
std::pair<KeyType, ValueType>& Map<KeyType, ValueType, Compare>::Iterator::operator*()
{
    if (node == map->tree.getSentinel())
        throw std::out_of_range("Cannot dereference end() iterator.");
    return node->KeyValuePair;
}

template<typename KeyType, typename ValueType, typename Compare>
std::pair<KeyType, ValueType>* Map<KeyType, ValueType, Compare>::Iterator::operator->()
{
    if (node == map->tree.getSentinel())
        throw std::out_of_range("Cannot dereference end() iterator.");
    return &(node->KeyValuePair);
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator& Map<KeyType, ValueType, Compare>::Iterator::operator=(const Iterator& other)
{
    if (this != &other)
    {
        node = other.node;
        map = other.map;
    }
    return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator& Map<KeyType, ValueType, Compare>::Iterator::prev()
{
    if (*this == Iterator(map->tree.min(map->tree.getRoot()), map))
        throw std::out_of_range("Cannot access previous element from the first element.");
    if (*this == map->end())
        node = map->tree.max(map->tree.getRoot());
    else
        node = map->tree.predecessor(node);
    if (!node)
        throw std::out_of_range("Cannot access previous element from the first element.");
    return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator& Map<KeyType, ValueType, Compare>::Iterator::next()
{
    if (*this == map->end())
        throw std::out_of_range("Cannot access next element from end().");
    if (node == map->tree.max(map->tree.getRoot()))
        node = map->end().node;
    else
        node = map->tree.successor(node);
    return *this;
}

/* ======================= ConstIterator Implementations ======================= */

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator& Map<KeyType, ValueType, Compare>::ConstIterator::operator=(const ConstIterator& other)
{
    if (this != &other)
    {
        node = other.node;
        map = other.map;
    }
    return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator& Map<KeyType, ValueType, Compare>::ConstIterator::operator++()
{
    if (node == map->tree.getSentinel())
        return *this;
    auto successorNode = map->tree.successor(const_cast<typename RBT<KeyType, ValueType, Compare>::Node*>(node));
    node = successorNode ? successorNode : map->tree.getSentinel();
    return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator Map<KeyType, ValueType, Compare>::ConstIterator::operator++(int)
{
    ConstIterator temp = *this;
    ++(*this);
    return temp;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator& Map<KeyType, ValueType, Compare>::ConstIterator::operator--()
{
    if (*this == ConstIterator(map->tree.min(map->tree.getRoot()), map))
        throw std::out_of_range("Cannot decrement const_iterator past the beginning.");
    if (node == map->tree.getSentinel())
    {
        node = map->tree.max(map->tree.getRoot());
        return *this;
    }
    auto predecessorNode = map->tree.predecessor(const_cast<typename RBT<KeyType, ValueType, Compare>::Node*>(node));
    if (!predecessorNode)
        throw std::out_of_range("Cannot decrement const_iterator past the beginning.");
    node = predecessorNode;
    return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator Map<KeyType, ValueType, Compare>::ConstIterator::operator--(int)
{
    ConstIterator temp = *this;
    --(*this);
    return temp;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator Map<KeyType, ValueType, Compare>::ConstIterator::operator+(int n)
{
    ConstIterator temp = *this;
    for (int i = 0; i < n; i++)
        ++temp;
    return temp;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator Map<KeyType, ValueType, Compare>::ConstIterator::operator-(int n)
{
    ConstIterator temp = *this;
    for (int i = 0; i < n; i++)
        --temp;
    return temp;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator& Map<KeyType, ValueType, Compare>::ConstIterator::operator+=(int n)
{
    if (n < 0)
    {
        while (n < 0)
        {
            ++n;
            --(*this);
        }
    }
    else
    {
        while (n > 0)
        {
            --n;
            ++(*this);
        }
    }
    return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator& Map<KeyType, ValueType, Compare>::ConstIterator::operator-=(int n)
{
    if (n < 0)
    {
        while (n < 0)
        {
            ++n;
            ++(*this);
        }
    }
    else
    {
        while (n > 0)
        {
            --n;
            --(*this);
        }
    }
    return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::ConstIterator::operator==(const ConstIterator& other) const
{
    return node == other.node && map == other.map;
}

template<typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::ConstIterator::operator!=(const ConstIterator& other) const
{
    return !(*this == other);
}

template<typename KeyType, typename ValueType, typename Compare>
const std::pair<KeyType, ValueType>& Map<KeyType, ValueType, Compare>::ConstIterator::operator*() const
{
    if (node == map->tree.getSentinel())
        throw std::out_of_range("Cannot dereference end() const_iterator.");
    return node->KeyValuePair;
}

template<typename KeyType, typename ValueType, typename Compare>
const std::pair<KeyType, ValueType>* Map<KeyType, ValueType, Compare>::ConstIterator::operator->() const
{
    if (node == map->tree.getSentinel())
        throw std::out_of_range("Cannot dereference end() const_iterator.");
    return &node->KeyValuePair;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator& Map<KeyType, ValueType, Compare>::ConstIterator::prev()
{
    if (*this == ConstIterator(map->tree.min(map->tree.getRoot()), map))
        throw std::out_of_range("Cannot access previous element from the first element with const_iterator.");
    if (*this == map->cend())
        node = map->tree.max(map->tree.getRoot());
    else
    {
        auto p = map->tree.predecessor(const_cast<typename RBT<KeyType, ValueType, Compare>::Node*>(node));
        if (!p)
            throw std::out_of_range("Cannot access previous element from the first element with const_iterator.");
        node = p;
    }
    return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator& Map<KeyType, ValueType, Compare>::ConstIterator::next()
{
    if (*this == map->cend())
        throw std::out_of_range("Cannot access next element from end() const_iterator.");
    if (node == map->tree.max(map->tree.getRoot()))
        node = map->end().node;
    else
    {
        auto s = map->tree.successor(const_cast<typename RBT<KeyType, ValueType, Compare>::Node*>(node));
        node = s ? s : map->end().node;
    }
    return *this;
}
