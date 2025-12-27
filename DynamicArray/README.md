# DynamicArray&lt;T&gt;

DynamicArray&lt;T&gt; is a resizable array container that stores elements in contiguous memory.<br/>
It provides fast random access, supports insertion and removal at any position, and automatically manages capacity as elements are added.

---

## Interface

### Constructors / Destructor
- `DynamicArray()` : Creates an empty array.
- `DynamicArray(std::size_t count)` : Creates array with count default-initialized elements.
- `DynamicArray(std::size_t count, const T& value)` : Create an array with count copies of value.
- `DynamicArray(std::initializer_list<T> init)` : Initializes from list {1, 2, 3}.
- `DynamicArray(const DynamicArray& other)` : Copy constructor.
- `DynamicArray(DynamicArray&& other)` : Move constructor.
- `~DynamicArray()` : Destroys all elements and frees memory.

### Assignment Operators
- `DynamicArray& operator=(const DynamicArray& other)` : Copy assignment.
- `DynamicArray& operator=(DynamicArray&& other)` : Move assignment.
- `DynamicArray& operator=(std::initializer_list<T> ilist)` : Initializer-list assignment.

### Element Access
- `T& At(std::size_t pos)` : Bounds-checked access.
- `const T& At(std::size_t pos)` : Const version bounds-checked access.
- `T& operator[](std::size_t pos)` : Unchecked element access.
- `const T& operator[](std::size_t pos)` : Const version unchecked access.
- `T& Front()` : Returns reference to first element.
- `const T& Front()` : Returns const reference to first element.
- `T& Back()` : Returns reference to last element.
- `const T& Back()` : Returns const reference to last element.
- `T* Data()` : Returns pointer to internal buffer.
- `const T* Data()` : Returns const pointer to internal buffer.

### Capacity
- `bool IsEmpty()` : Returns true if container is empty.
- `std::size_t Size()` : Returns number of stored elements.
- `std::size_t Capacity()` : Returns allocated capacity.
- `void Reserve(std::size_t newCapacity)` : Ensures capacity is at least given size.
- `void ShrinkToFit()` : Reduces capacity to match current size.

### Modifiers
- `void Clear()` : Removes all elements.
- `void Insert(std::size_t pos, const T& value)` : Inserts copy of value at pos.
- `void Insert(std::size_t pos, T&& value)` : Moves value into array at pos.
- `void Insert(std::size_t pos, std::size_t count, const T& value)` : Inserts count copies of value at pos.
- `void Insert(std::size_t pos, std::initializer_list<T> ilist)` : Inserts elements from initializer-list at pos.
- `T& Emplace(std::size_t pos, Args&&... args)` : Constructs element in-place at pos.
- `void Erase(std::size_t pos)` : Removes element at pos.
- `void Erase(std::size_t pos, std::size_t count)` : Removes count elements starting from pos.
- `void PushBack(const T& value)` : Appends a copy of value.
- `void PushBack(T&& value)` : Moves value to the end of array.
- `T& EmplaceBack(Args&&... args)` : Constructs an element in place.
- `void PopBack()` : Removes last element.
- `void Resize(std::size_t newSize)` : Resizes using default initialization.
- `void Resize(std::size_t newSize, const T& value)` : Resizes and fills new elements with value.
- `void Swap(DynamicArray& other)` : Exchanges contents with another array.

### Non-member Functions
- `bool operator==(const DynamicArray& lhs, const DynamicArray& rhs)` : Equality comparison.
- `auto operator<=>(const DynamicArray& lhs, const DynamicArray& rhs)` : Lexicographical comparison.

---

## Usage Example

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

    // Insert at specific positions
    arr.Insert(2, 99);              // Insert 99 at index 2
    arr.Insert(0, 2, 0);            // Insert two 0's at beginning
    
    // Insert multiple values
    arr.Insert(arr.Size() - 1, {100, 101, 102});
    
    // Remove elements
    arr.Erase(0, 2);                // Remove first two elements
    arr.Erase(2);                   // Remove element at index 2

    std::cout << "Front: " << arr.Front() << "\n";
    std::cout << "Back:  " << arr.Back()  << "\n";
    std::cout << "Size:  " << arr.Size()  << "\n";

    // Iterate
    std::cout << "Elements: ";
    for (std::size_t i = 0; i < arr.Size(); ++i)
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
