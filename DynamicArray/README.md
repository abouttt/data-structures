# DynamicArray&lt;T&gt;

`DynamicArray<T>` is a resizable container that stores elements in contiguous memory.<br/>
It provides fast random access, efficient insertion and removal at the end of the sequence, automatically grows its internal capacity as new elements are added.

---

# Interface

### Constructors / Destructor / Assignment
- `DynamicArray()` : Creates an empty array.
- `DynamicArray(size_t)` : Creates an array with reserved capacity.
- `DynamicArray(std::initializer_list<T>)` : Initializes from `{1, 2, 3}`.
- `DynamicArray(const DynamicArray&)` : Copy constructor.
- `DynamicArray(DynamicArray&&)` : Move constructor.
- `operator=` : Copy, move, or initializer-list assignment.
- `~DynamicArray()` : Destroys all elements and frees memory.

### Element Access
- `At(size_t)` : Bounds-checked access.
- `operator[](size_t)` : Unchecked element access.
- `Front()` : Returns the first element.
- `Back()` : Returns the last element.
- `Data()` : Returns a pointer to the internal buffer.

### Capacity
- `IsEmpty()` : Returns true if the container is empty.
- `Size()` : Returns the number of stored elements.
- `Capacity()` : Returns the allocated capacity.
- `Reserve(size_t)` : Ensures capacity is at least the given size.
- `ShrinkToFit()` : Reduces capacity to match the current size.

### Modifiers
- `Clear()` : Removes all elements.
- `PushBack(const T&)` : Appends an element (copy).
- `PushBack(T&&)` : Appends an element (move).
- `EmplaceBack(Args&&...)` : Constructs an element in place.
- `PopBack()` : Removes the last element.
- `Resize(size_t)` : Resizes using default initialization.
- `Resize(size_t, const T&)` : Resizes and fills new elements with a value.
- `Swap(DynamicArray&)` : Exchanges contents with another array.

### Non-member Functions
- `operator==` : Equality comparison.
- `operator<=>` : Lexicographical comparison.

---

# Usage

```cpp
#include <iostream>
#include "DynamicArray.h"

int main()
{
    // Create and initialize
    DynamicArray<int> arr = {1, 2, 3};

    // Append elements
    arr.PushBack(4);
    arr.EmplaceBack(5);

    std::cout << "Front: " << arr.Front() << "\n"; // 1
    std::cout << "Back:  " << arr.Back()  << "\n"; // 5
    std::cout << "Size:  " << arr.Size()  << "\n"; // 5

    // Iterate
    std::cout << "Elements: ";
    for (size_t i = 0; i < arr.Size(); ++i)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // Manage capacity
    arr.Reserve(100);
    std::cout << "Capacity: " << arr.Capacity() << "\n";
    
    arr.ShrinkToFit();
    std::cout << "Capacity after shrink: " << arr.Capacity() << "\n";

    return 0;
}
