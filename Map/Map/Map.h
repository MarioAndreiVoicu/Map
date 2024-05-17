#pragma once
#include"RBT.h"

template <typename KeyType, typename ValueType, typename Compare = std::less<KeyType>>
class Map
{
	RBT<KeyType, ValueType, Compare> tree;

	Compare comparator;

public:
	class Iterator;
	Map(const Compare& comparator = Compare());

	Map(const Map& other, const Compare& comparator = Compare());

	Map(Iterator itBegin, Iterator itEnd, const Compare& comparator = Compare());

	Map(const std::vector<std::pair<KeyType, ValueType>>& values, const Compare& Comparator = Compare());

	~Map() {}

	typename RBT<KeyType, ValueType, Compare>::TreeNode* find(const KeyType& key);

	ValueType& operator[](const KeyType& key);

	void insert(const std::pair<KeyType, ValueType>& KeyValue);

	bool empty();

	int size();

	int count(const KeyType& key);

	std::pair<KeyType, ValueType>& max();

	std::pair<KeyType, ValueType>& min();

	void erase(const KeyType& keyDeleted);

	void clear();

	void display();

	bool operator==(Map<KeyType, ValueType, Compare>& other);

	bool operator!=(const RBT<KeyType, ValueType, Compare>& other);

	void mergeMaps(Map<KeyType, ValueType, Compare>& mergeMap);

	Iterator begin();

	Iterator end();

	Iterator lower_bound(const KeyType& key);

	Iterator upper_bound(const KeyType& key);

	class Iterator
	{
		typename RBT<KeyType, ValueType, Compare>::TreeNode* node;
		Map& map;

	public:
		Iterator(typename RBT<KeyType, ValueType, Compare>::TreeNode* nodePointer, typename Map<KeyType, ValueType, Compare>& mapReference)
			: node(nodePointer), map(mapReference) {}

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

		std::pair<KeyType, ValueType>& operator*();

		std::pair<KeyType, ValueType>* operator->();

		Iterator& operator=(const Iterator& other);

		Iterator& prev();

		Iterator& next();
	};
};

template<typename KeyType, typename ValueType, typename Compare>
Map<KeyType, ValueType, Compare>::Map(const Compare& comparator)
{
	this->comparator = comparator;
}

template<typename KeyType, typename ValueType, typename Compare>
Map<KeyType, ValueType, Compare>::Map(const Map& other, const Compare& comparator)  //copy constructor
{
	*this = other;
	this->comparator = comparator;
}

template<typename KeyType, typename ValueType, typename Compare>
Map<KeyType, ValueType, Compare>::Map(Iterator itBegin, Iterator itEnd, const Compare& comparator)  //range constructor
{
	for (Iterator it = itBegin; it != itEnd; ++it)
	{
		tree.insert((*it));
	}

}

template<typename KeyType, typename ValueType, typename Compare>
Map<KeyType, ValueType, Compare>::Map(const std::vector<std::pair<KeyType, ValueType>>& values, const Compare& Comparator)  //constructor with vector of values for the map calls the constructor with vector of values of the tree
	: tree(values), comparator(Comparator)
{}

template<typename KeyType, typename ValueType, typename Compare>
typename RBT<KeyType, ValueType, Compare>::TreeNode* Map<KeyType, ValueType, Compare>::find(const KeyType& key)
{
	return tree.find(key);
}

template<typename KeyType, typename ValueType, typename Compare>
ValueType& Map<KeyType, ValueType, Compare>::operator[](const KeyType& key)
{
	return tree[key];
}

template<typename KeyType, typename ValueType, typename Compare>
void Map<KeyType, ValueType, Compare>::insert(const std::pair<KeyType, ValueType>& KeyValue)
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
bool Map<KeyType, ValueType, Compare>::operator==(Map<KeyType, ValueType, Compare>& other)  //two maps are equal if they have the same KeyValue pairs,so it checks if the map's trees have the same KeyValuePairs stored.It returns true if the trees have different structures but contain the same elements
{  //it iterates trough the maps in parallel and checks if the KeyValue pairs are equal
	if (tree.size() != other.tree.size())
		return false;

	Map<KeyType, ValueType, Compare>::Iterator it1 = begin();
	Map<KeyType, ValueType, Compare>::Iterator it2 = other.begin();

	while (it1 != end())
	{
		if (it1 != it2)  //checks if the iterators have the same KeyValue
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
void Map<KeyType, ValueType, Compare>::mergeMaps(Map<KeyType, ValueType, Compare>& mergeMap)  //merges two maps into the map that is the first parameter
{
	Map<KeyType, ValueType, Compare>::Iterator it = mergeMap.begin();

	for (; it != mergeMap.end(); ++it)
	{
		std::pair<KeyType, ValueType> pair = *it;
		tree.insert(pair);
	}
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::begin()
{
	return Iterator(tree.min(tree.getRoot()), *this);
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::end()
{
	return Iterator(tree.getSentinelNode(), *this);
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::lower_bound(const KeyType& key)  //find the first element with the key equal to or greater than the parameter key
{
	typename RBT<KeyType, ValueType, Compare>::TreeNode* lowerNode = nullptr;
	typename RBT<KeyType, ValueType, Compare>::TreeNode* node = tree.getRoot();

	while (node != tree.getSentinelNode())
	{
		if (node->KeyValuePair.first == key)
			return Iterator(node, *this);

		if (node->KeyValuePair.first > key)
		{
			lowerNode = node;
			node = node->leftNode;
		}
		else
		{
			node = node->rightNode;
		}
	}

	if (lowerNode == nullptr)
		return end();  //if there is no key in the map equal to or greater than the parameter key

	return Iterator(lowerNode, *this);
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::upper_bound(const KeyType& key)  //find the first element with the key greater than the parameter key
{
	typename RBT<KeyType, ValueType, Compare>::TreeNode* upperNode = nullptr;
	typename RBT<KeyType, ValueType, Compare>::TreeNode* node = tree.getRoot();

	while (node != tree.getSentinelNode())
	{
		if (node->KeyValuePair.first > key)
		{
			upperNode = node;
			node = node->leftNode;
		}
		else
		{
			node = node->rightNode;
		}
	}

	if (upperNode == nullptr)
		return end();  //if there is no key in the map equal to or greater than the parameter key

	return Iterator(upperNode, *this);
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator& Map<KeyType, ValueType, Compare>::Iterator::operator++()  //pre-increment
{
	if (node == nullptr)  //if operator++ was called on a nullptr,it returns the end of the map
	{
		*this = map.end();
		return *this;
	}

	if (*this == map.end())
		return *this;

	node = map.tree.successor(node);  //the next elemenet is the successor

	if (node == nullptr)  //if it doesn't have a successor,it it the last element in the map and the iterator points to the end
		*this = map.end();

	return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::Iterator::operator++(int)  //post-increment
{
	Iterator temp = *this;
	++(*this);
	return temp;  //it returns the value before the incrementation,but the interator is incremented
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator& Map<KeyType, ValueType, Compare>::Iterator::operator--()  //pre-decrement
{
	if (node == nullptr)
		throw std::out_of_range("Cannot decrement iterator pointing to the beginning of the map.");

	node = map.tree.predecessor(node);

	if (node == nullptr)  //if it doesn't have a predecessor,it is the first element in the map and there is no element before it
		throw std::out_of_range("Cannot decrement iterator pointing to the beginning of the map.");

	return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::Iterator::operator--(int)  //post-decrement
{
	Iterator temp = *this;
	--(*this);
	return temp;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::Iterator::operator+(int n)  //goes forward n elements
{
	for (int index = 0; index < n; ++index)
		++(*this);

	return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::Iterator::operator-(int n)  //goes backwards n elements
{
	for (int index = 0; index < n; ++index)
		--(*this);

	return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator& Map<KeyType, ValueType, Compare>::Iterator::operator+=(int n)
{
	if (n < 0)  //if the iterator goes backwards
	{
		while (n)
		{
			++n;
			--(*this);
		}
	}
	else  //if the iterator goes forward
	{
		while (n)
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
	if (n < 0)  //if the iterator goes forward
	{
		while (n)
		{
			++n;
			++(*this);
		}
	}
	else  //if the iterator goes backwards
	{
		while (n)
		{
			--n;
			--(*this);
		}
	}

	return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::Iterator::operator==(const Iterator& other) const  //two iterators are considered equal if the nodes have equal KeyValue pairs
{
	return node->KeyValuePair == other.node->KeyValuePair;
}

template<typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::Iterator::operator!=(const Iterator& other) const
{
	return !(*this == other);
}

template<typename KeyType, typename ValueType, typename Compare>
std::pair<KeyType, ValueType>& Map<KeyType, ValueType, Compare>::Iterator::operator*()
{
	return node->KeyValuePair;
}

template<typename KeyType, typename ValueType, typename Compare>
std::pair<KeyType, ValueType>* Map<KeyType, ValueType, Compare>::Iterator::operator->()
{
	return &(node->KeyValuePair);
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator& Map<KeyType, ValueType, Compare>::Iterator::operator=(const Iterator& other)
{
	if (this != &other)  //if the iterators are different,then "this" iterator takes the value of "other"
	{
		node = other.node;
		map = other.map;
	}

	return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator& Map<KeyType, ValueType, Compare>::Iterator::prev()
{
	if (node == nullptr)
		throw std::out_of_range("Cannot access previous element from an iterator pointing to nullptr.");

	if (node == map.end().node)  //if the prev function is called on the iterator pointing to the end,then a valid iterator that points to the last element of the map is returned
		node = map.tree.max(map.tree.getRoot());  //the last element is the max from the tree
	else
		node = map.tree.predecessor(node);

	if (node == nullptr)  //if the predecessor function was called on the first element of the map
		throw std::out_of_range("Cannot access previous element from an iterator pointing to the first element.");

	return *this;
}

template<typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator& Map<KeyType, ValueType, Compare>::Iterator::next()
{
	if (node == nullptr)
		throw std::out_of_range("Cannot access next element from an iterator pointing to nullptr.");

	if (node == map.end().node)  //end is the same as the sentinel node
		throw std::out_of_range("Cannot access next element from an iterator pointing to the sentinel node.");

	if (node == map.tree.max(map.tree.getRoot()))
		node = map.end().node;
	else
		node = map.tree.successor(node);

	return *this;
}