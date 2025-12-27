#include <cassert>
#include <compare>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "DynamicArray.h"

void RunTest(const std::string& name, void(*func)())
{
	std::cout << "[TEST] " << name << " ... ";
	func();
	std::cout << "PASSED" << std::endl;
}

void TestConstructors()
{
	// Default constructor
	DynamicArray<int> arr1;
	assert(arr1.Size() == 0);
	assert(arr1.Capacity() == 0);

	// Fill constructor
	DynamicArray<int> arr2(5, 999);
	assert(arr2.Size() == 5);
	assert(arr2[0] == 999);
	assert(arr2[4] == 999);

	// Initializer list constructor
	DynamicArray<int> arr3 = { 1, 2, 3, 4, 5 };
	assert(arr3.Size() == 5);
	assert(arr3.Capacity() == 5);
	assert(arr3[0] == 1);
	assert(arr3[4] == 5);

	// Copy constructor
	DynamicArray<int> arr4 = arr3;
	assert(arr4.Size() == 5);
	assert(arr4.Capacity() == 5);
	assert(arr4[0] == 1);
	assert(arr4[4] == 5);

	// Ensure deep copy
	arr4[0] = 10;
	assert(arr3[0] == 1);

	// Move constructor
	DynamicArray<int> arr5 = std::move(arr4);

	// Ensure arr5 has the data
	assert(arr5.Size() == 5);
	assert(arr5.Capacity() == 5);
	assert(arr5.Data() != nullptr);

	// Ensure arr4 is in a valid state after move
	assert(arr4.Size() == 0);
	assert(arr4.Capacity() == 0);
	assert(arr4.Data() == nullptr);
}

void TestAssignments()
{
	DynamicArray<int> arr1 = { 1, 2, 3 };
	DynamicArray<int> arr2;

	// Copy assignment
	arr2 = arr1;
	assert(arr2.Size() == 3);
	assert(arr2.Capacity() == 3);
	assert(arr2[0] == 1);
	assert(arr2[2] == 3);

	// Ensure deep copy
	arr2[0] = 10;
	assert(arr1[0] == 1);

	// Move assignment
	DynamicArray<int> arr3;
	arr3 = std::move(arr2);

	// Ensure arr3 has the data
	assert(arr3.Size() == 3);
	assert(arr3.Capacity() == 3);
	assert(arr3.Data() != nullptr);

	// Ensure arr2 is in a valid state after move
	assert(arr2.Size() == 0);
	assert(arr2.Capacity() == 0);
	assert(arr2.Data() == nullptr);

	// Initializer list assignment
	arr2 = { 4, 5, 6 };
	assert(arr2.Size() == 3);
	assert(arr2.Capacity() == 3);
	assert(arr2[0] == 4);
	assert(arr2[2] == 6);
}

void TestElementAccess()
{
	DynamicArray<int> arr = { 10, 20, 30, 40, 50 };

	// operator[]
	assert(arr[0] == 10);
	assert(arr[2] == 30);
	assert(arr[4] == 50);

	arr[0] = 15;
	assert(arr[0] == 15);

	// At()
	assert(arr.At(1) == 20);
	arr.At(1) = 25;
	assert(arr.At(1) == 25);

	bool exceptionThrown = false;
	try
	{
		arr.At(5); // Out of range
	}
	catch (const std::out_of_range&)
	{
		exceptionThrown = true;
	}

	// Front(), Back()
	assert(arr.Front() == 15);
	assert(arr.Back() == 50);
	arr.Front() = 12;
	arr.Back() = 55;
	assert(arr.Front() == 12);
	assert(arr.Back() == 55);

	// Data()
	int* data = arr.Data();
	assert(data[2] == 30);
	data[2] = 35;
	assert(arr[2] == 35);
}

void TestCapacity()
{
	DynamicArray<int> arr;

	// IsEmpty()
	assert(arr.IsEmpty());

	// Size(), Capacity()
	assert(arr.Size() == 0);
	assert(arr.Capacity() == 0);

	// Reserve()
	arr.Reserve(10);
	assert(arr.Capacity() == 10);

	// PushBack to increase size
	for (int i = 0; i < 5; ++i)
	{
		arr.PushBack(i);
	}
	assert(!arr.IsEmpty());
	assert(arr.Size() == 5);
	assert(arr.Capacity() == 10);

	// ShrinkToFit()
	size_t oldCapacity = arr.Capacity();
	arr.ShrinkToFit();
	assert(arr.Capacity() == arr.Size());
	assert(arr.Capacity() < oldCapacity);
}

void TestModifiers()
{
	DynamicArray<int> arr;

	// PushBack()
	arr.PushBack(1);
	assert(arr.Size() == 1);
	assert(arr[0] == 1);

	// EmplaceBack()
	arr.EmplaceBack(2);
	assert(arr.Size() == 2);
	assert(arr[1] == 2);

	// Insert()
	arr.Insert(0, 0); // 0, 1, 2
	assert(arr.Size() == 3);
	assert(arr[0] == 0);

	arr.Insert(1, 3, 999); // 0, 999, 999, 999, 1, 2
	assert(arr.Size() == 6);
	assert(arr[1] == 999);
	assert(arr[3] == 999);

	arr.Insert(6, { 4, 5, 6 }); // 0, 999, 999, 999, 1, 2, 4, 5, 6
	assert(arr.Size() == 9);
	assert(arr[6] == 4);
	assert(arr[8] == 6);

	// Emplace()
	arr.Emplace(3, 555); // 0, 999, 999, 555, 999, 1, 2, 4, 5, 6
	assert(arr.Size() == 10);
	assert(arr[3] == 555);

	// Erase()
	arr.Erase(3); // 0, 999, 999, 999, 1, 2, 4, 5, 6
	assert(arr.Size() == 9);
	assert(arr[3] == 999);

	arr.Erase(1, 3); // 0, 1, 2, 4, 5, 6
	assert(arr.Size() == 6);
	assert(arr[1] == 1);
	assert(arr[3] == 4);

	// PopBack()
	arr.PopBack(); // 0, 1, 2, 4, 5
	assert(arr.Size() == 5);
	assert(arr.Back() == 5);

	// Resize()
	arr.Resize(10); // 0, 1, 2, 4, 5, 0, 0, 0, 0, 0
	assert(arr.Size() == 10);
	assert(arr[5] == 0);

	arr.Resize(3); // 0, 1, 2
	assert(arr.Size() == 3);
	assert(arr[0] == 0);
	assert(arr[1] == 1);
	assert(arr[2] == 2);

	// Clear()
	arr.Clear();
	assert(arr.IsEmpty());
	assert(arr.Size() == 0);
	assert(arr.Capacity() > 0);
}

void TestComparisons()
{
	DynamicArray<int> a = { 1, 2, 3 };
	DynamicArray<int> b = { 1, 2, 3 };
	DynamicArray<int> c = { 1, 2, 4 };
	DynamicArray<int> d = { 1, 2 };
	DynamicArray<int> empty1;
	DynamicArray<int> empty2;

	// operator==
	assert(a == b);
	assert(!(a == c));
	assert(!(a == d));
	assert(empty1 == empty2);

	// operator<=>
	assert((a <=> b) == std::strong_ordering::equal);
	assert((a <=> c) == std::strong_ordering::less);
	assert((c <=> a) == std::strong_ordering::greater);
	assert((a <=> d) == std::strong_ordering::greater);
	assert((d <=> a) == std::strong_ordering::less);
	assert((empty1 <=> empty2) == std::strong_ordering::equal);
}

int main()
{
	std::cout << "Starting DynamicArray tests\n\n";

	RunTest("Constructors", TestConstructors);
	RunTest("Assignments", TestAssignments);
	RunTest("ElementAccess", TestElementAccess);
	RunTest("Capacity", TestCapacity);
	RunTest("Modifiers", TestModifiers);
	RunTest("Comparisons", TestComparisons);

	std::cout << "\nAll tests passed successfully!\n";

	return 0;
}
