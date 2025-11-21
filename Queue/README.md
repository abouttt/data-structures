# Queue

`Queue` is a circular-buffer–based FIFO (First-In, First-Out) container implemented in C++.  
It supports constant-time insertion at the back, removal from the front, and automatic capacity growth while preserving element order.  
Internally, it manages memory manually using raw allocation and placement construction.

---

# Interface

The following interface provides constructors, accessors, capacity utilities, and queue modification operations.

### Constructors / Destructor / Assignment
- `Queue()` — Creates an empty queue.  
- `Queue(size_t capacity)` — Reserves initial capacity.  
- `Queue(std::initializer_list<T>)` — Initializes from `{1, 2, 3}`.  
- `Queue(const Queue&)` — Deep copy constructor.  
- `Queue(Queue&&)` — Move constructor.  
- `operator=` — Copy and move assignment operators.  
- `~Queue()` — Destroys all elements and frees memory.  

---

### Element Access
- `Front()` — Returns the first (oldest) element.  
- `Back()` — Returns the last (most recently inserted) element.  

---

### Capacity
- `GetSize()` — Returns the number of stored elements.  
- `IsEmpty()` — Returns whether the queue is empty.  

---

### Modifiers
- `Push(const T&)` — Inserts a copy of a value at the back.  
- `Push(T&&)` — Moves a value into the back.  
- `Emplace(args...)` — Constructs an element directly in place at the back.  
- `Pop()` — Removes the front element.  
- `Swap(other)` — Exchanges internal buffers, indices, size, and capacity.  

---

### Comparison Operators
- `operator==` — Compares elements for equality in FIFO order.  
- `operator<=>` — Performs lexicographical three-way comparison.  

---

# Usage

```cpp
#include <iostream>
#include "Queue.h"

int main()
{
    // Create an empty queue
    Queue<int> q;

    // Push elements into the queue
    q.Push(1);
    q.Push(2);
    q.Emplace(3);   // [1, 2, 3]

    // Access front and back
    std::cout << "Front: " << q.Front() << "\n"; // 1
    std::cout << "Back:  " << q.Back()  << "\n"; // 3

    // Pop the first element
    q.Pop(); // [2, 3]

    // Insert more values (wrap-around will occur automatically)
    for (int i = 0; i < 10; ++i)
    {
        q.Push(i);
    }

    std::cout << "Front after wrap: " << q.Front() << "\n";
    std::cout << "Back after wrap:  " << q.Back()  << "\n";

    return 0;
}
