# LinkedList

`LinkedList` is a doubly linked list container implemented in C++.  
Each node stores `Next`, `Prev`, and `Data`.  
The structure supports fast insertion/removal, deep copying, moving, resizing, and comparison.

---

# Interface

The following interface provides constructors, accessors, capacity utilities, and modification operations.

### Constructors / Destructor / Assignment
- `LinkedList()` — Creates an empty list.  
- `LinkedList(std::initializer_list<T>)` — Initializes from `{1, 2, 3}`.  
- `LinkedList(const LinkedList&)` — Deep copy constructor.  
- `LinkedList(LinkedList&&)` — Move constructor.  
- `operator=` — Copy/move assignment.  
- `~LinkedList()` — Destroys all nodes.

---

### Element Access
- `Front()` — Returns the first element.  
- `Back()` — Returns the last element.

---

### Capacity
- `GetSize()` — Returns number of nodes.  
- `IsEmpty()` — Returns true if size is zero.

---

### Modifiers
- `PushFront(const T&)` — Inserts at the front.  
- `PushFront(T&&)` — Inserts by move at the front.  
- `EmplaceFront(args...)` — Constructs in place at the front.
- `PushBack(const T&)` — Inserts at the back.  
- `PushBack(T&&)` — Inserts by move at the back.  
- `EmplaceBack(args...)` — Constructs in place at the back.
- `PopFront()` — Removes first element.  
- `PopBack()` — Removes last element.
- `Resize(size_t)` — Resizes with default values.  
- `Resize(size_t, const T&)` — Resizes and fills.
- `Clear()` — Removes all nodes.  
- `Swap(other)` — Exchanges pointers and size.

---

### Comparison Operators
- `operator==` — Node-by-node equality comparison.  
- `operator<=>` — Lexicographical comparison.

---

# Usage

```cpp
#include <iostream>
#include "LinkedList.h"

int main()
{
    // Create a list with initial elements
    LinkedList<int> list = {1, 2, 3};          // [1, 2, 3]

    // Insert at both ends
    list.PushFront(0);                         // [0, 1, 2, 3]
    list.PushBack(4);                          // [0, 1, 2, 3, 4]

    // Inspect first and last elements
    std::cout << "Front: " << list.Front() << "\n"; // 0
    std::cout << "Back:  " << list.Back()  << "\n"; // 4
    std::cout << "Size:  " << list.GetSize() << "\n";

    // Remove from both ends
    list.PopFront();                           // [1, 2, 3, 4]
    list.PopBack();                            // [1, 2, 3]

    // Grow the list, filling new nodes with 10
    list.Resize(5, 10);                        // [1, 2, 3, 10, 10]

    // Simple readout of front/back after operations
    std::cout << "Front after resize: " << list.Front() << "\n"; // 1
    std::cout << "Back after resize:  " << list.Back()  << "\n"; // 10

    return 0;
}
