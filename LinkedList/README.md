# LinkedList

`LinkedList` is a doubly linked container implemented in C++.  
Each node stores `Next`, `Prev`, and a `Data` value, enabling fast insertion and removal at both ends of the list.  
The container supports deep copying, moving, resizing, comparison, and basic list operations.

---

# Interface

The following functions provide element access, capacity queries, and modification utilities for operating on a linked list.

### Constructors / Destructor / Assignment
- `LinkedList()` — Creates an empty list  
- `LinkedList(std::initializer_list<T>)` — Initializes from `{1,2,3}`  
- `LinkedList(const LinkedList&)` — Deep copy constructor  
- `LinkedList(LinkedList&&)` — Move constructor  
- `operator=` — Copy and move assignment  
- `~LinkedList()` — Destroys all nodes  

---

### Element Access
- `Front()` — Returns the first element  
- `Back()` — Returns the last element  

---

### Capacity
- `GetSize()` — Returns the number of elements  
- `IsEmpty()` — Returns whether the list is empty  

---

### Modifiers
- `PushFront(const T&)` — Inserts at the front  
- `PushFront(T&&)` — Inserts by move at the front  
- `EmplaceFront(args...)` — Constructs an element at the front  

- `PushBack(const T&)` — Inserts at the back  
- `PushBack(T&&)` — Inserts by move at the back  
- `EmplaceBack(args...)` — Constructs an element at the back  

- `PopFront()` — Removes the first element  
- `PopBack()` — Removes the last element  

- `Resize(size_t)` — Resizes using default construction  
- `Resize(size_t, const T&)` — Resizes and fills new nodes with a value  

- `Clear()` — Removes all nodes  
- `Swap(other)` — Exchanges internal resources  

---

### Comparison Operators
- `operator==` — Compares lists for equality  
- `operator<=>` — Lexicographical three-way comparison  

These are implemented as friend functions for efficient access to internal node data.

---

# Usage

```cpp
#include <iostream>
#include "LinkedList.h"

int main()
{
    LinkedList<int> list = {1, 2, 3};

    list.PushFront(0);
    list.PushBack(4);

    std::cout << "Front: " << list.Front() << "\n";  // 0
    std::cout << "Back:  " << list.Back()  << "\n";  // 4
    std::cout << "Size:  " << list.GetSize() << "\n"; // 5

    list.PopFront();
    list.PopBack();

    std::cout << "Front: " << list.Front() << "\n";  // 1
    std::cout << "Back:  " << list.Back()  << "\n";  // 3

    return 0;
}
