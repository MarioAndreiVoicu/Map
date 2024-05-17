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
    // Creating a Map object that stores pairs with an integer key and a string value
    Map<int, std::string> map;  // or use a custom comparator Map<int, std::string, CustomComparator> map

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