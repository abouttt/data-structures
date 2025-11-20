# DynamicArray

`DynamicArray` is a custom C++ dynamic array container that provides automatic resizing, element access, and efficient insertion at the end of the array.  
It manually manages memory using raw allocation and placement construction, offering behavior similar to `std::vector`.

---

# Interface

The following interface provides constructors, accessors, capacity utilities, and modification operations.

### Constructors / Destructor / Assignment
- `DynamicArray()` — Creates an empty array.  
- `DynamicArray(size_t capacity)` — Reserves initial capacity.  
- `DynamicArray(std::initializer_list<T>)` — Initializes from `{1, 2, 3}`.  
- `DynamicArray(const DynamicArray&)` — Deep copy constructor.  
- `DynamicArray(DynamicArray&&)` — Move constructor.  
- `operator=` — Copy/move assignment.  
- `~DynamicArray()` — Destroys elements and frees memory.

---

### Element Access
- `operator[](size_t)` — Fast unchecked access.  
- `At(size_t)` — Bounds-checked access.  
- `Front()` — Returns the first element.  
- `Back()` — Returns the last element.  
- `GetData()` — Returns pointer to internal buffer.

---

### Capacity
- `GetSize()` — Returns number of elements.  
- `GetCapacity()` — Returns allocated capacity.  
- `IsEmpty()` — Returns true if empty.  
- `Reserve(size_t)` — Ensures capacity.  
- `ShrinkToFit()` — Shrinks to match size.

---

### Modifiers
- `Clear()` — Removes all elements.  
- `PushBack(const T&)` — Inserts by copy.  
- `PushBack(T&&)` — Inserts by move.  
- `EmplaceBack(args...)` — Constructs in place.  
- `PopBack()` — Removes last element.  
- `Resize(size_t)` — Resizes with default values.  
- `Resize(size_t, const T&)` — Resizes and fills.  
- `Swap(other)` — Exchanges internal data.

---

### Comparison Operators
- `operator==` — Equality comparison.  
- `operator<=>` — Lexicographical comparison.

---

# Usage

```cpp
#include <iostream>
#include "DynamicArray.h"

int main()
{
    // Create and initialize with a braced-init list
    DynamicArray<int> arr = {1, 2, 3};

    // Append elements at the end
    arr.PushBack(4);        // [1, 2, 3, 4]
    arr.EmplaceBack(5);     // [1, 2, 3, 4, 5]

    // Access front/back information
    std::cout << "Front: " << arr.Front() << "\n"; // 1
    std::cout << "Back:  " << arr.Back()  << "\n"; // 5
    std::cout << "Size:  " << arr.GetSize() << "\n";

    // Iterate over all elements using operator[]
    std::cout << "Elements: ";
    for (size_t i = 0; i < arr.GetSize(); ++i)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // Resize the array, filling new slots with 10
    arr.Resize(8, 10);      // [1, 2, 3, 4, 5, 10, 10, 10]

    std::cout << "After Resize: ";
    for (size_t i = 0; i < arr.GetSize(); ++i)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    return 0;
}
