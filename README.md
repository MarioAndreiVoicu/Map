# Map Class

The Map class is a key-value mapping container similar to `std::map`, implemented using a Red-Black Tree internally. It provides efficient insertion, deletion, and searching operations with a balanced tree structure.

## Features

- **Key-Value Mapping**: Stores key-value pairs where keys are unique and ordered.
- **Red-Black Tree Implementation**: Utilizes a Red-Black Tree data structure.
- **Efficient Operations**: Offers logarithmic time complexity for insertion, deletion, and searching operations.
- **Iterator Support**: Provides iterators for traversal and manipulation of elements.
- **Templated**: This map class is templated, allowing the storage of various types for both keys and values. Additionally, the comparator is also templated, providing flexibility in sorting behavior.
- **Multiple Constructors**: Includes default constructor, copy constructor, range constructor, and constructor with a vector of values.
- **Custom Comparator**: Offers the option to define a custom comparator for the map's key comparison operations.
- **Operator Overloading**: Overloaded operators like : operator=, operator[], operator==, operator!=,

## Functionality

### Constructors:
  - Default Constructor
  - Copy Constructor
  - Range Constructor
  - Vector Constructor

### Operators:
  - `operator=`: Assignment operator
  - `operator[]`: Subscript operator
  - `operator==`: Equality operator
  - `operator!=`: Inequality operator

### Capacity:
- `empty`: Checks if the map is empty.
- `size`: Returns the number of elements in the map.

### Modifiers:
- `insert`: Inserts an element into the map.
- `erase`: Removes an element from the map.
- `clear`: Removes all elements from the map.
- `mergeMaps`: Merges another map into this map.

### Lookup:
- `find`: Finds an element with a specified key.
- `count`: Counts the number of elements with a specified key.

### Iterator operations:
- `begin`: Returns an iterator to the beginning.
- `end`: Returns an iterator to the end.
- `lower_bound`: Returns an iterator to the first element not less than the given key.
- `upper_bound`: Returns an iterator to the first element greater than the given key.

### Observers:
- `max`: Returns the maximum key in the map.
- `min`: Returns the minimum key in the map.

 ## Iterator

- **Prefix Increment (++)**: Advances the iterator to the next element in the sequence and returns a reference to the updated iterator.
  
- **Postfix Increment (++)**: Advances the iterator to the next element in the sequence but returns a copy of the iterator as it was before the increment.

- **Prefix Decrement (--)**: Moves the iterator to the previous element in the sequence and returns a reference to the updated iterator.
  
- **Postfix Decrement (--)**: Moves the iterator to the previous element in the sequence but returns a copy of the iterator as it was before the decrement.

- **Dereference (*)**: Returns a reference to the KeyValuePair of the element pointed to by the iterator.

- **Member Access Arrow (->)**: Allows access to the KeyValuePair pointed to by the iterator.

- **Compound Assignment Operators (+=, -=)**: These operators allow you to modify the iterator's position by a given offset.
  
- **Equality and Inequality Operators (== , !=)**: These operators check if two iterators have the same KeyValue pair.

## Example Usage

```cpp
#include "Map.h"

struct CustomComparator  // Functor
{
    bool operator()(int a, int b)
    {
        return a > b; // Sort in descending order
    }
};

int main()
{
    // Creating a Map object
    Map<int, std::string, CustomComparator> map;  // or just use Map<int, std::string> map for the std::less comparator,which will sort the elements ascending

   // Inserting elements
    map.insert({ 1, "Apple" });
    map.insert({ 2, "Orange" });
    map.insert({ 3, "Banana" });

    // Iterate over the map using the Iterator
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        std::cout << "Key: " << it->first << ", Value: " << it->second << "\n";
    }

    // Accessing elements
    std::cout << "Value at key 2: " << map[2] << "\n";

    // Deleting an element
    map.erase(2);

    return 0;
}
