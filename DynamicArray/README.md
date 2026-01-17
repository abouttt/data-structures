# DynamicArray&lt;T&gt;

`DynamicArray<T>` is a resizable array container that stores elements in contiguous memory.  
It provides fast random access, supports insertion and removal at arbitrary positions, and automatically manages its capacity as elements are added.

---

## Complexity

- Random access : $O(1)$
- Insertion or removal at the end : Amortized $O(1)$
- Insertion or removal in the middle : $O(N)$
- Search : $O(N)$

---

## Interface

### Constants

- `constexpr std::size_t INDEX_NONE` (static)  
  Sentinel value returned when an element is not found.

### Constructors / Destructor

- `DynamicArray()`  
  Constructs an empty array.

- `DynamicArray(std::size_t count)`  
  Constructs an array with `count` value-initialized elements.

- `DynamicArray(std::size_t count, const T& value)`  
  Constructs an array with `count` copies of `value`.

- `DynamicArray(std::initializer_list<T> init)`  
  Constructs an array from an initializer list.

- `DynamicArray(const DynamicArray& other)`  
  Copy constructor. Performs deep copy of `other`.

- `DynamicArray(DynamicArray&& other)`  
  Move constructor. Transfers ownership from `other`.

- `~DynamicArray()`  
  Destroys all elements and deallocates memory.

### Assignment Operators

- `DynamicArray& operator=(const DynamicArray& other)`  
  Copy assignment operator. Uses copy-and-swap idiom.

- `DynamicArray& operator=(DynamicArray&& other)`  
  Move assignment operator.

- `DynamicArray& operator=(std::initializer_list<T> ilist)`  
  Assigns values from an initializer list.

### Element Access

- `T& operator[](std::size_t index)`  
  Returns a reference to the element at `index` without bounds checking.

- `const T& operator[](std::size_t index)`  
  Const version of `operator[]`.

- `T& At(std::size_t index)`  
  Returns a reference to the element at `index` with bounds checking.  
  Throws `std::out_of_range` if `index >= Size()`.

- `const T& At(std::size_t index)`  
  Const version of `At()`.

- `T* Data()`  
  Returns a pointer to the underlying array.

- `const T* Data()`  
  Const version of `Data()`.

### Capacity

- `bool IsEmpty()`  
  Returns `true` if the array contains no elements.

- `std::size_t Size()`  
  Returns the number of elements.

- `std::size_t Capacity()`  
  Returns the current allocated capacity.

- `void Reserve(std::size_t newCapacity)`  
  Allocates storage for at least `newCapacity` elements.  
  Does nothing if `newCapacity <= Capacity()`.

- `void Shrink()`  
  Reduces capacity to match the current size.  
  Does nothing if `Size() == Capacity()`.

### Modifiers

- `void Add(const T& value)`  
  Appends a copy of `value` to the end.

- `void Add(T&& value)`  
  Appends `value` using move semantics.

- `void Add(std::initializer_list<T> ilist)`  
  Appends all elements from the initializer list to the end.

- `T& Emplace(Args&&... args)`  
  Constructs an element in-place at the end.  
  Returns reference to the constructed element.

- `T& EmplaceAt(std::size_t index, Args&&... args)`  
  Constructs an element in-place at position `index`.  
  Returns reference to the constructed element.

- `void Insert(std::size_t index, const T& value)`  
  Inserts a copy of `value` at position `index`.  
  Throws `std::out_of_range` if `index > Size()`.

- `void Insert(std::size_t index, T&& value)`  
  Inserts `value` at position `index` using move semantics.  
  Throws `std::out_of_range` if `index > Size()`.

- `void Insert(std::size_t index, std::initializer_list<T> ilist)`  
  Inserts elements from the initializer list starting at `index`.  
  Throws `std::out_of_range` if `index > Size()`.

- `bool Remove(const T& value)`  
  Removes the first occurrence of `value`.  
  Returns `true` if an element was removed.

- `void RemoveAt(std::size_t index)`  
  Removes the element at `index`.  
  Throws `std::out_of_range` if `index >= Size()`.

- `void RemoveAt(std::size_t index, std::size_t count)`  
  Removes `count` consecutive elements starting at `index`.  
  If `index + count > Size()`, removes all elements from `index` to the end.

- `std::size_t RemoveAll(Predicate pred)`  
  Removes all elements satisfying the predicate `pred`.  
  Returns the number of removed elements.

- `void Clear()`  
  Removes all elements. Capacity remains unchanged.

- `void Resize(std::size_t newSize)`  
  Resizes the array. New elements are value-initialized.

- `void Resize(std::size_t newSize, const T& value)`  
  Resizes the array. New elements are copies of `value`.

- `void Swap(DynamicArray& other)`  
  Exchanges contents with `other`.

### Lookup

- `bool Contains(const T& value)`  
  Returns `true` if the array contains `value`.

- `bool ContainsIf(Predicate pred)`  
  Returns `true` if any element satisfies the predicate `pred`.

- `std::size_t Find(const T& value)`  
  Returns the index of the first occurrence of `value`, or `INDEX_NONE` if not found.

- `std::size_t FindIf(Predicate pred)`  
  Returns the index of the first element satisfying the predicate `pred`, or `INDEX_NONE` if not found.

- `std::size_t FindLast(const T& value)`  
  Returns the index of the last occurrence of `value`, or `INDEX_NONE` if not found.

- `std::size_t FindLastIf(Predicate pred)`  
  Returns the index of the last element satisfying the predicate `pred`, or `INDEX_NONE` if not found.

### Non-member Functions

- `bool operator==(const DynamicArray& lhs, const DynamicArray& rhs)`  
  Returns `true` if both arrays have the same size and all elements compare equal.

- `auto operator<=>(const DynamicArray& lhs, const DynamicArray& rhs)`  
  Performs lexicographical comparison using three-way comparison.

---

## Usage Example

```cpp
#include <iostream>
#include "DynamicArray.h"

int main()
{
    // Create and initialize using initializer list
    DynamicArray<int> arr = {1, 2, 3};               // [1, 2, 3]
    
    // Append elements (Add supports both copy and move)
    arr.Add(4);                                      // [1, 2, 3, 4]
    arr.Emplace(5);                                  // [1, 2, 3, 4, 5]
    
    // Insert elements at specific positions
    arr.Insert(2, 99);                               // [1, 2, 99, 3, 4, 5]
    arr.Insert(0, {100, 101});                       // [100, 101, 1, 2, 99, 3, 4, 5]
    
    // Remove elements by index or value
    arr.RemoveAt(0, 2);                              // Remove first two: [1, 2, 99, 3, 4, 5]
    arr.Remove(99);                                  // Remove by value: [1, 2, 3, 4, 5]
    
    // Search using built-in lookup methods
    std::size_t index = arr.Find(3);
    if (index != DynamicArray<int>::INDEX_NONE)
    {
        std::cout << "Found at index: " << index << "\n";  // Output: Found at index: 2
    }
    
    // Manage memory manually if needed
    arr.Reserve(100);     // Capacity becomes 100
    arr.Shrink();         // Capacity becomes 5 (matches Size)
    
    // Safe access with exception handling
    try
    {
        int value = arr.At(10); 
    }
    catch (const std::out_of_range& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }
    
    return 0;
}
