# DynamicArray

`DynamicArray` is a custom C++ dynamic array container that provides automatic resizing, element access, and efficient insertion at the end of the array.  
It manually manages memory using raw allocation and placement construction, offering behavior similar to `std::vector` while exposing the underlying mechanisms more explicitly.

---

# Interface

The following functions provide element access, capacity management, and modification utilities for operating on a custom dynamic array.

### Constructors / Destructor / Assignment
- `DynamicArray()` — Creates an empty array  
- `DynamicArray(size_t capacity)` — Reserves initial capacity  
- `DynamicArray(std::initializer_list<T>)` — Initializes from `{1,2,3}`  
- `DynamicArray(const DynamicArray&)` — Copy constructor  
- `DynamicArray(DynamicArray&&)` — Move constructor  
- `operator=` — Copy and move assignment  
- `~DynamicArray()` — Destroys all elements and releases memory  

---

### Element Access
- `operator[](size_t)` — Fast unchecked access with debug bounds validation  
- `At(size_t)` — Bounds-checked access (throws on invalid index)  
- `Front()` — Returns the first element  
- `Back()` — Returns the last element  
- `GetData()` — Returns a pointer to the internal buffer  

---

### Capacity
- `GetSize()` — Returns the number of elements  
- `GetCapacity()` — Returns the current capacity  
- `IsEmpty()` — Returns whether the container is empty  
- `Reserve(size_t)` — Expands the capacity  
- `ShrinkToFit()` — Reduces capacity to match size  

---

### Modifiers
- `Clear()` — Removes all elements while keeping capacity  
- `PushBack(const T&)` — Inserts by copy  
- `PushBack(T&&)` — Inserts by move  
- `EmplaceBack(args...)` — Constructs an element in place  
- `PopBack()` — Removes the last element  
- `Resize(size_t)` — Resizes with default construction  
- `Resize(size_t, const T&)` — Resizes and fills with a value  
- `Swap(other)` — Exchanges internal resources  

---

### Comparison Operators
- `operator==` — Compares stacks for equality  
- `operator<=>` — Lexicographical three-way comparison  

These are implemented as friend functions for efficient access to private storage.

---

# Usage

```cpp
#include <iostream>
#include "DynamicArray.h"

int main()
{
    // Initialization
    DynamicArray<int> arr = {1, 2, 3};

    // Insert elements
    arr.PushBack(4);
    arr.EmplaceBack(5);

    // Access
    std::cout << "Front: " << arr.Front() << "\n";   // 1
    std::cout << "Back:  " << arr.Back()  << "\n";   // 5
    std::cout << "Size:  " << arr.GetSize() << "\n"; // 5

    // Iterate
    for (size_t i = 0; i < arr.GetSize(); ++i)
    {
        std::cout << arr[i] << " ";
    }

    // Resize
    arr.Resize(8, 10);

    std::cout << "\nAfter resize: ";
    for (size_t i = 0; i < arr.GetSize(); ++i)
    {
        std::cout << arr[i] << " ";
    }

    return 0;
}
