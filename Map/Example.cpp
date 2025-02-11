#include "Map.h"
#include <iostream>

// Custom comparator for descending order
struct DescendingComparator
{
    bool operator()(int a, int b) const
    {
        return a > b;
    }
};

int main()
{
    // ===============================
    // Example 1: Default Comparator (Ascending)
    // ===============================
    {
        Map<int, std::string> map;
        map.insert({ 1, "Apple" });
        map.insert({ 2, "Orange" });
        map.insert({ 3, "Banana" });

        std::cout << "Ascending order (default comparator):\n";
        for (auto it = map.begin(); it != map.end(); ++it)
            std::cout << "Key: " << it->first << ", Value: " << it->second << "\n";

        auto lb = map.lower_bound(2);
        if (lb != map.end())
            std::cout << "lower_bound(2): Key: " << lb->first << ", Value: " << lb->second << "\n";

        auto ub = map.upper_bound(2);
        if (ub != map.end())
            std::cout << "upper_bound(2): Key: " << ub->first << ", Value: " << ub->second << "\n";
        else
            std::cout << "upper_bound(2) reached end.\n";
    }

    std::cout << "\n";

    // ===============================
    // Example 2: Custom Comparator (Descending)
    // ===============================
    {
        Map<int, std::string, DescendingComparator> descendingMap;
        descendingMap.insert({ 1, "Apple" });
        descendingMap.insert({ 2, "Orange" });
        descendingMap.insert({ 3, "Banana" });

        std::cout << "Descending order (custom comparator):\n";
        for (auto it = descendingMap.begin(); it != descendingMap.end(); ++it)
            std::cout << "Key: " << it->first << ", Value: " << it->second << "\n";

        auto lb = descendingMap.lower_bound(2);
        if (lb != descendingMap.end())
            std::cout << "lower_bound(2): Key: " << lb->first << ", Value: " << lb->second << "\n";

        auto ub = descendingMap.upper_bound(2);
        if (ub != descendingMap.end())
            std::cout << "upper_bound(2): Key: " << ub->first << ", Value: " << ub->second << "\n";
        else
            std::cout << "upper_bound(2) reached end.\n";
    }

    return 0;
}
