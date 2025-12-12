# LinkedList&lt;T&gt;

`LinkedList` is a bidirectional linked container that stores elements in non-contiguous memory.<br/>
It provides efficient insertion and removal at both the front and back, supports bidirectional traversal, and allows dynamic resizing of the sequence.

---

# Interface

### Constructors / Destructor / Assignment
- `LinkedList()` : Creates an empty list.
- `LinkedList(std::initializer_list<T>)` : Initializes from `{1, 2, 3}`.
- `LinkedList(const LinkedList&)` : Copy constructor.
- `LinkedList(LinkedList&&)` : Move constructor.
- `operator=` : Copy, move, or initializer-list assignment.
- `~LinkedList()` : Destroys all nodes.

---

### Element Access
- `Front()` : Returns the first element.
- `Back()` : Returns the last element.

---

### Capacity
- `Size()` : Returns number of nodes.
- `IsEmpty()` : Returns true if size is zero.

---

### Modifiers
- `Clear()` : Removes all nodes.
- `PushBack(const T&)` : Inserts at the back (copy).
- `PushBack(T&&)` : Inserts at the back (move).
- `EmplaceBack(args...)` : Constructs in place at the back.
- `PopBack()` : Removes last element.
- `PushFront(const T&)` : Inserts at the front (copy).
- `PushFront(T&&)` : Inserts at the front (move).
- `EmplaceFront(args...)` : Constructs in place at the front.
- `PopFront()` : Removes first element.
- `Resize(size_t)` : Resizes using default initialization.
- `Resize(size_t, const T&)` : Resizes and fills new elements with a value.
- `Swap(LinkedList&)` : Exchanges contents with another linked list.

---

### Non-member Functions
- `operator==` — Node-by-node equality comparison.
- `operator<=>` — Lexicographical comparison.

---

# Usage

```cpp
#include <iostream>
#include "LinkedList.h"

int main()
{
    // Create and initialize
    LinkedList<int> list = {1, 2, 3};

    // Insert at both ends
    list.PushFront(0);
    list.PushBack(4);

    std::cout << "Front: " << list.Front() << "\n"; // 0
    std::cout << "Back:  " << list.Back()  << "\n"; // 4
    std::cout << "Size:  " << list.GetSize() << "\n";

    // Remove from both ends
    list.PopFront();
    list.PopBack();

    std::cout << "Front: " << list.Front() << "\n"; // 1
    std::cout << "Back:  " << list.Back()  << "\n"; // 3
    std::cout << "Size:  " << list.GetSize() << "\n";

    return 0;
}
