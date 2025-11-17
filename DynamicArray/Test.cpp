#include <cassert>
#include <iostream>

#include "DynamicArray.h"

int main()
{
    DynamicArray<int> arr;

    // PushBack test
    arr.PushBack(1);
    arr.PushBack(2);
    arr.PushBack(3);

    assert(arr.GetSize() == 3);
    assert(arr.Front() == 1);
    assert(arr.Back() == 3);

    // Operator[] test
    assert(arr[0] == 1);
    assert(arr[1] == 2);

    // Resize larger
    arr.Resize(5, 10);
    assert(arr.GetSize() == 5);
    assert(arr[3] == 10);

    // Resize smaller
    arr.Resize(2);
    assert(arr.GetSize() == 2);

    // Copy constructor
    DynamicArray<int> copy(arr);
    assert(copy == arr);

    // Move constructor
    DynamicArray<int> moved(std::move(copy));
    assert(moved.GetSize() == 2);

    // ShrinkToFit test
    moved.Reserve(100);
    moved.ShrinkToFit();
    assert(moved.GetCapacity() == moved.GetSize());

    std::cout << "All DynamicArray tests passed!\n";
}
