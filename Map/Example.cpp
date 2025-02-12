import Map;
#include <iostream>
#include <string>

// Custom comparator: sorts fruit names in descending order.
struct DescendingComparator {
    bool operator()(const std::string& a, const std::string& b) const {
        return a > b;
    }
};

int main() {
    using namespace std;

    // --- Default Fruit Inventory (Ascending Order) ---
    Map<string, int> fruitInventory;
    fruitInventory.insert({ "Apple", 50 });
    fruitInventory.insert({ "Banana", 30 });
    fruitInventory.insert({ "Cherry", 20 });
    fruitInventory["Date"] = 40;       // Insert via operator[]
    fruitInventory["Banana"] = 35;     // Update existing key

    cout << "Fruit Inventory (Ascending Order):\n";
    for (auto it = fruitInventory.begin(); it != fruitInventory.end(); ++it)
        cout << it->first << ": " << it->second << "\n";
    cout << "\n";

    // --- Lookup Operations & Bounds ---
    if (auto it = fruitInventory.find("Cherry"); it != fruitInventory.end())
        cout << "Found 'Cherry' with quantity: " << it->second << "\n";

    auto lb = fruitInventory.lower_bound("Date");
    if (lb != fruitInventory.end())
        cout << "Lower bound for 'Date': " << lb->first << "\n";
    auto ub = fruitInventory.upper_bound("Date");
    if (ub != fruitInventory.end())
        cout << "Upper bound for 'Date': " << ub->first << "\n";
    cout << "\n";

    // --- Erase an Element ---
    fruitInventory.erase("Apple");
    cout << "After erasing 'Apple':\n";
    for (auto it = fruitInventory.begin(); it != fruitInventory.end(); ++it)
        cout << it->first << ": " << it->second << "\n";
    cout << "\n";

    // --- Merge with Another Map ---
    Map<string, int> extraFruits;
    extraFruits.insert({ "Elderberry", 15 });
    extraFruits.insert({ "Fig", 25 });
    fruitInventory.mergeMaps(extraFruits);
    cout << "After merging extra fruits:\n";
    for (auto it = fruitInventory.begin(); it != fruitInventory.end(); ++it)
        cout << it->first << ": " << it->second << "\n";
    cout << "\n";

    // --- Copy and Move Semantics ---
    Map<string, int> copyInventory = fruitInventory;                 // Copy constructor
    Map<string, int> movedInventory = std::move(copyInventory);      // Move constructor
    cout << "Moved Inventory:\n";
    for (auto it = movedInventory.begin(); it != movedInventory.end(); ++it)
        cout << it->first << ": " << it->second << "\n";
    cout << "Copy Inventory is empty: " << boolalpha << copyInventory.empty() << "\n\n";

    // --- Equality Comparison ---
    Map<string, int> compareInventory = fruitInventory;
    cout << "fruitInventory " << (fruitInventory == compareInventory ? "==" : "!=")
        << " compareInventory\n\n";

    // --- Clear the Map ---
    fruitInventory.clear();
    cout << "After clearing, fruitInventory empty: " << fruitInventory.empty() << "\n\n";

    // --- Custom Comparator Map (Descending Order) ---
    Map<string, int, DescendingComparator> fruitInventoryDesc;
    fruitInventoryDesc.insert({ "Apple", 50 });
    fruitInventoryDesc.insert({ "Banana", 30 });
    fruitInventoryDesc.insert({ "Cherry", 20 });
    fruitInventoryDesc["Date"] = 40;
    fruitInventoryDesc["Banana"] = 35;

    cout << "Fruit Inventory (Descending Order):\n";
    for (auto it = fruitInventoryDesc.begin(); it != fruitInventoryDesc.end(); ++it)
        cout << it->first << ": " << it->second << "\n";
    cout << "\n";

    return 0;
}