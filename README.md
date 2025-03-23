# C++ Red-Black Tree & Map Implementation

A C++ implementation from scratch of a map container similar to `std::map` that uses a red–black tree internally.

---

## Overview

- **Red–Black Tree:**  
  A self-balancing binary search tree with O(log n) operations. Core operations such as rotations, repairs, and traversals are implemented using a sentinel node strategy.

- **Map Container:**  
  Wraps the red–black tree to provide an interface similar to `std::map`. Supports custom comparators, full iterator support, operator overloading, etc.

- **Modern C++:**  
  Uses C++20 modules, templates, and move semantics.

---

## Key Features

- **Iterators:**

  - Provides both `Iterator` and `ConstIterator` with support for increment, decrement, and basic arithmetic.

- **Templated Design:**

  - `template <typename KeyType, typename ValueType, typename Compare = std::less<KeyType>>`.
  - Supports arbitrary key and value types, and allows for a custom comparator to control key ordering.

- **Cross-Platform Potential:**

  - Built using `CMake` to make it cross-platform. (Tested with the Visual Studio generator on Windows; compatibility with other generators/platforms is unverified)

- **Comprehensive Operations**

  - `insert`, `erase`, `clear`, `size`, `empty`, `find`, and more.
  - Range queries like `lower_bound`, `upper_bound`.
  - Merging support with `mergeMaps()`.

- **Operator Overloads**
  - Comparisons: `==`, `!=`.
  - Element access: `[]`.
  - Assignment & move semantics: copy/move constructor, `operator=`, etc.

---

## Functionality

### Constructors & Assignment

- **Map(const Compare&)** – Default constructor.
- **Map(const Map&)**, **Map(Map&&)**, **operator=()** – Copy and move operations.

### Basic Operations

- **find(key)** – Returns an iterator to the element with the given key.
- **operator[](key)** – Accesses/inserts an element by key.
- **insert(pair)** – Inserts a key–value pair.
- **erase(key)** – Removes the element with the specified key.
- **clear()** – Clears the map.
- **empty()** – Checks if the map is empty.
- **size()** – Returns the number of elements.

### Range & Query Operations

- **min() / max()** – Returns an iterator to the smallest/largest element.
- **lower_bound(key) / upper_bound(key)** – Provides range queries.

### Comparison & Merging

- **operator== / operator!=** – Equality comparisons.
- **mergeMaps(other)** – Merges another map into this one.

### Iterators

- **begin(), end(), cbegin(), cend()** – Standard iterator functions.
- Custom iterator support for navigation (e.g., `++`, `--`, `+`, `-`).

---

## Build & Run Instructions

**Prerequisites**:

- C++20-compatible compiler
- CMake 3.26 or later
  > **Note:** This project currently works with the Visual Studio generator. The Ninja generator does not work and compatibility with other generators is not verified.

### Building the Library and Demo

1. **Clone the repository**
   ```bash
    git clone https://github.com/MarioAndreiVoicu/Map.git
   ```
2. **Create and enter a build directory in the project root**
   ```bash
   mkdir build && cd build
   ```
3. **Generate build files with CMake (using the Visual Studio generator)**
   ```bash
    cmake -G "Visual Studio 17 2022" ..
   ```
   > **Note:** If you prefer to use a different generator, adjust the command accordingly, but note that only the Visual Studio generator has been confirmed to work.
4. **Build the project**
   ```bash
   cmake --build .
   ```

### Running the Demo

Either open the `.sln` file and set MapExample as the startup project, <br />
or <br />
Search for and run `MapDemo.exe` from the build directory. <br />
The demo prints the results of the example below.

```cpp
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
```
