# Stack&lt;T&gt;

`Stack<T>` is a LIFO (Last-In, First-Out) container that stores elements in contiguous memory.<br/>
It provides fast insertion and removal at the top, automatically grows its internal capacity as new elements are added.

---

# Interface

### Constructors / Destructor / Assignment
- `Stack()` : Creates an empty stack.
- `Stack(size_t)` : Creates a stack with reserved capacity.
- `Stack(std::initializer_list<T>)` : Initializes from `{1, 2, 3}`.
- `Stack(const Stack&)` : Copy constructor.
- `Stack(Stack&&)` : Move constructor.
- `operator=` : Copy, move, or initializer-list assignment.
- `~Stack()` : Destroys all elements and frees memory.

### Element Access
- `Top()` : Returns the top element.

### Capacity
- `IsEmpty()` : Returns whether the stack is empty.
- `Size()` : Returns the number of stored elements.
- `Capacity()` : Returns the allocated capacity.
- `Reserve(size_t)` : Ensures capacity is at least the given size.
- `ShrinkToFit()` : Reduces capacity to match the current size.

### Modifiers
- `Clear()` : Removes all elements.
- `Push(const T&)` : Pushes an element (copy).
- `Push(T&&)` : Pushes an element (move).
- `Emplace(Args&&...)` : Constructs an element at the top.
- `Pop()` : Removes the top element.
- `Swap(Stack&)` : Exchanges contents with another stack.

### Non-member Functions
- `operator==` : Equality comparison.
- `operator<=>` : Lexicographical comparison.

---

# Usage

```cpp
#include <iostream>
#include "Stack.h"

int main()
{
    // Create and initialize
    Stack<int> s = {1, 2, 3}; // top = 3

    // Push new elements
    s.Push(4);
    s.Emplace(5);

    std::cout << "Top:  " << s.Top()  << "\n"; // 5
    std::cout << "Size: " << s.Size() << "\n"; // 5

    // Pop one element
    s.Pop();
    std::cout << "After Pop, Top: " << s.Top() << "\n"; // 4

    // Clear and reuse
    s.Clear();
    std::cout << "IsEmpty: " << s.IsEmpty() << "\n";

    return 0;
}
