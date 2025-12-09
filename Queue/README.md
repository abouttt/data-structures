# Queue&lt;T&gt;

`Queue<T>` is a FIFO (First-In, First-Out) container that stores elements in a circular buffer.<br/>
It provides efficient insertion at the back, fast removal from the front, and automatically expands its internal capacity as new elements are enqueued.

---

# Interface

### Constructors / Destructor / Assignment
- `Queue()` : Creates an empty queue.
- `Queue(size_t)` : Creates a queue with reserved capacity.
- `Queue(std::initializer_list<T>)` : Initializes from `{1, 2, 3}`.  
- `Queue(const Queue&)` : Copy constructor.  
- `Queue(Queue&&)` : Move constructor.  
- `operator=` : Copy, move, or initializer-list assignment.
- `~Queue()` : Destroys all elements and frees memory.

### Element Access
- `Front()` : Returns the first (oldest) element.
- `Back()` : Returns the last (newest) element.

### Capacity
- `IsEmpty()` : Returns true if the queue is empty.
- `Size()` : Returns the number of stored elements.
- `Capacity()` : Returns the allocated capacity.
- `Reserve(size_t)` : Ensures capacity is at least the given size.
- `ShrinkToFit()` : Reduces capacity to match the current size.

### Modifiers
- `Clear()` : Removes all elements.
- `PushBack(const T&)` : Pushes an element (copy).
- `PushBack(T&&)` : Pushes an element (move).
- `Emplace(Args&&...)` : Constructs an element in place.
- `Pop()` : Removes the first element.
- `Swap(Queue&)` : Exchanges contents with another queue.

### Non-member Functions
- `operator==` : Equality comparison.  
- `operator<=>` : Lexicographical comparison.

---

# Usage

```cpp
#include <iostream>
#include "Queue.h"

int main()
{
    // Create and initialize
    Queue<int> q = {1, 2, 3};

    // Push elements
    q.Push(4);
    q.Emplace(5);

    std::cout << "Front: " << q.Front() << "\n"; // 1
    std::cout << "Back:  " << q.Back()  << "\n"; // 5
    std::cout << "Size:  " << q.Size()  << "\n"; // 5

    // Pop
    q.Pop(); // removes 1
    std::cout << "After Pop, Front: " << q.Front() << "\n"; // 2

    // Clear and reuse
    q.Clear();
    std::cout << "IsEmpty: " << q.IsEmpty() << "\n";

    return 0;
}
