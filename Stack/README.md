# Stack

`Stack` is a simple array-based stack container implemented in C++.  
It manages its own memory using raw allocation and placement-new, providing efficient push and pop operations.  
The container grows automatically as elements are added and supports copy/move semantics, element access, and comparison operations.

---

# Interface

The following interface provides fundamental stack operations including insertion, removal, element access, and capacity queries.

### Constructors / Destructor / Assignment
- `Stack()` — Creates an empty stack  
- `Stack(size_t capacity)` — Reserves initial capacity  
- `Stack(std::initializer_list<T>)` — Initializes from `{1, 2, 3}`  
- `Stack(const Stack&)` — Copy constructor  
- `Stack(Stack&&)` — Move constructor  
- `operator=` — Copy and move assignment  
- `~Stack()` — Destroys all elements and frees memory  

---

### Element Access
- `Top()` — Returns a reference to the top element  
- `const Top()` — Returns a const reference to the top element  

---

### Capacity
- `IsEmpty()` — Checks whether the stack is empty  
- `GetSize()` — Returns the number of stored elements  

---

### Modifiers
- `Push(const T&)` — Pushes an element by copy  
- `Push(T&&)` — Pushes an element by move  
- `Emplace(args...)` — Constructs an element directly at the top  
- `Pop()` — Removes the top element  
- `Swap(other)` — Exchanges internal resources with another stack  

---

### Comparison Operators
- `operator==` — Compares stacks for equality  
- `operator<=>` — Lexicographical three-way comparison  

These are implemented as friend functions for efficient access to private storage.

---

# Usage


```cpp
#include "Stack.h"
#include <cassert>
#include <iostream>

int main()
{
    Stack<int> s;

    // Push elements
    s.Push(1);
    s.Push(2);
    s.Push(3);

    assert(s.Top() == 3);
    assert(s.GetSize() == 3);

    // Pop element
    s.Pop();
    assert(s.Top() == 2);

    // Emplace an element
    s.Emplace(10);
    assert(s.Top() == 10);

    // Copy stack
    Stack<int> copy(s);
    assert(copy.Top() == s.Top());

    // Move stack
    Stack<int> moved(std::move(copy));
    assert(moved.GetSize() == s.GetSize());

    std::cout << "Stack example finished successfully.\n";
    return 0;
}
