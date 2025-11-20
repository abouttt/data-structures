# Stack

`Stack` is a simple array-based stack container implemented in C++.  
It grows dynamically as elements are pushed and provides efficient access to the top element.  
The container supports deep copying, moving, and comparison.

---

# Interface

The following interface provides constructors, accessors, capacity utilities, and stack modification operations.

### Constructors / Destructor / Assignment
- `Stack()` — Creates an empty stack.  
- `Stack(size_t capacity)` — Allocates initial capacity.  
- `Stack(std::initializer_list<T>)` — Initializes from `{1, 2, 3}`.  
- `Stack(const Stack&)` — Deep copy constructor.  
- `Stack(Stack&&)` — Move constructor.  
- `operator=` — Copy/move assignment.  
- `~Stack()` — Destroys all elements.

---

### Element Access
- `Top()` — Returns the top element.

---

### Capacity
- `GetSize()` — Returns number of elements.  
- `IsEmpty()` — Returns true if size is zero.

---

### Modifiers
- `Push(const T&)` — Pushes a copy onto the stack.  
- `Push(T&&)` — Pushes a moved value.  
- `Emplace(args...)` — Constructs in place at the top.  
- `Pop()` — Removes the top element.  
- `Swap(other)` — Exchanges internal buffers.

---

### Comparison Operators
- `operator==` — Compares all elements.  
- `operator<=>` — Lexicographical comparison.

---

# Usage

```cpp
#include <iostream>
#include "Stack.h"

int main()
{
    // Create an empty stack of ints
    Stack<int> s;

    // Push a few values on the stack (LIFO order)
    s.Push(1);          // stack: [1]
    s.Push(2);          // stack: [1, 2]
    s.Push(3);          // stack: [1, 2, 3]

    // Inspect the current top element
    std::cout << "Top:  " << s.Top()     << "\n"; // 3
    std::cout << "Size: " << s.GetSize() << "\n"; // 3

    // Pop the top element
    s.Pop();            // stack: [1, 2]

    // Emplace constructs the value directly on top
    s.Emplace(10);      // stack: [1, 2, 10]

    // Check the new top
    std::cout << "Top after Emplace: " << s.Top() << "\n"; // 10

    // Optional: simple emptiness check
    if (!s.IsEmpty())
    {
        std::cout << "Stack is not empty.\n";
    }

    return 0;
}
