#include <cassert>
#include <compare>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "DynamicArray.h"

void RunTest(const std::string& name, void (*func)())
{
	std::cout << "[TEST] " << name << " ... ";
	func();
	std::cout << "PASSED" << std::endl;
}

void TestConstructors()
{
	// Default constructor
	{
		DynamicArray<int> arr;
		assert(arr.Size() == 0);
		assert(arr.Capacity() == 0);
		assert(arr.Data() == nullptr);
	}

	// Capacity constructor
	{
		DynamicArray<int> arr(10);
		assert(arr.Size() == 0);
		assert(arr.Capacity() == 10);
		assert(arr.Data() != nullptr);
	}

	// Initializer list constructor
	{
		DynamicArray<int> arr{ 1, 2, 3, 4, 5 };
		assert(arr.Size() == 5);
		assert(arr.Capacity() == 5);
		assert(arr[0] == 1);
		assert(arr[4] == 5);
	}

	// Copy constructor
	{
		DynamicArray<int> original{ 1, 2, 3 };
		DynamicArray<int> copy(original);

		assert(original.Size() == copy.Size());
		assert(original.Capacity() == copy.Capacity());
		assert(original.Data() != copy.Data()); // Deep copy
		for (size_t i = 0; i < original.Size(); ++i)
		{
			assert(copy[i] == original[i]);
		}
	}

	// Move constructor
	{
		DynamicArray<int> original{ 1, 2, 3 };
		size_t originalSize = original.Size();
		size_t originalCapacity = original.Capacity();
		int* originalData = original.Data();

		DynamicArray<int> moved(std::move(original));

		assert(moved.Size() == originalSize);
		assert(moved.Capacity() == originalCapacity);
		assert(moved.Data() == originalData);
		assert(original.Size() == 0);
		assert(original.Capacity() == 0);
		assert(original.Data() == nullptr);
	}
}

void TestAssignmentOperators()
{
	// Copy assignment operator
	{
		DynamicArray<int> original{ 1, 2, 3 };
		DynamicArray<int> copy;
		copy = original;

		assert(original.Size() == copy.Size());
		assert(original.Data() != copy.Data()); // Deep copy
		for (size_t i = 0; i < original.Size(); ++i)
		{
			assert(original[i] == copy[i]);
		}
	}

	// Move assignment operator
	{
		DynamicArray<int> original{ 1, 2, 3 };
		size_t originalSize = original.Size();
		size_t originalCapacity = original.Capacity();
		int* originalData = original.Data();

		DynamicArray<int> moved;
		moved = std::move(original);

		assert(moved.Size() == originalSize);
		assert(moved.Capacity() == originalCapacity);
		assert(moved.Data() == originalData);
		assert(original.Size() == 0);
		assert(original.Capacity() == 0);
		assert(original.Data() == nullptr);
	}

	// Initializer list assignment operator
	{
		DynamicArray<int> arr;
		arr = { 1, 2, 3, 4, 5 };

		assert(arr.Size() == 5);
		assert(arr[0] == 1);
		assert(arr[4] == 5);
	}
}

void TestElementAccess()
{
	DynamicArray<int> arr{ 10, 20, 30, 40, 50 };

	// operator[]
	assert(arr[0] == 10);
	assert(arr[2] == 30);
	assert(arr[4] == 50);

	arr[1] = 200;
	assert(arr[1] == 200);

	// At()
	assert(arr.At(0) == 10);
	assert(arr.At(4) == 50);

	arr.At(3) = 400;
	assert(arr.At(3) == 400);

	bool exceptionThrown = false;
	try
	{
		arr.At(10);
	}
	catch (const std::out_of_range&)
	{
		exceptionThrown = true;
	}
	assert(exceptionThrown);

	// Front(), Back()
	assert(arr.Front() == 10);
	assert(arr.Back() == 50);

	arr.Front() = 100;
	arr.Back() = 500;
	assert(arr.Front() == 100);
	assert(arr.Back() == 500);

	// Data()
	int* data = arr.Data();
	assert(data != nullptr);
	assert(*data == arr.Front());
	assert(*(data + arr.Size() - 1) == arr.Back());
}

void TestCapacity()
{
	// IsEmpty()
	{
		DynamicArray<int> emptyArr;
		DynamicArray<int> nonEmptyArr{ 1 };

		assert(emptyArr.IsEmpty() == true);
		assert(nonEmptyArr.IsEmpty() == false);
	}

	// Size(), Capacity()
	{
		DynamicArray<int> arr;
		assert(arr.Size() == 0);
		assert(arr.Capacity() == 0);

		arr.PushBack(1);
		assert(arr.Size() == 1);
		assert(arr.Capacity() >= 1);

		arr.PushBack(2);
		arr.PushBack(3);
		assert(arr.Size() == 3);
	}

	// Reserve()
	{
		DynamicArray<int> arr;
		arr.Reserve(100);
		assert(arr.Capacity() == 100);
		assert(arr.Size() == 0);

		// When capacity is already sufficient
		size_t oldCapacity = arr.Capacity();
		arr.Reserve(oldCapacity / 2);
		assert(arr.Capacity() == oldCapacity);  // Capacity should not shrink
	}

	// ShrinkToFit()
	{
		DynamicArray<int> arr;
		for (int i = 0; i < 10; ++i)
		{
			arr.PushBack(i);
		}

		size_t sizeBeforeShrink = arr.Size();
		size_t capacityBeforeShrink = arr.Capacity();
		arr.ShrinkToFit();

		assert(arr.Size() == sizeBeforeShrink);
		assert(arr.Size() == arr.Capacity());  // Capacity should equal size
		assert(arr.Capacity() < capacityBeforeShrink); // Should not grow
		for (int i = 0; i < 10; ++i)
		{
			assert(arr[i] == i);
		}
	}
}

void TestModifiers()
{
	// Clear()
	{
		DynamicArray<int> arr{ 1, 2, 3, 4, 5 };
		size_t capacityBeforeClear = arr.Capacity();
		arr.Clear();
		assert(arr.Size() == 0);
		assert(arr.Capacity() == capacityBeforeClear); // Capacity should be preserved
	}

	// PushBack() (const lvalue reference)
	{
		DynamicArray<int> arr;
		for (int i = 0; i < 100; ++i)
		{
			arr.PushBack(i);
		}

		assert(arr.Size() == 100);
		for (int i = 0; i < 100; ++i)
		{
			assert(arr[i] == i);
		}
	}

	// PushBack() (rvalue reference)
	{
		DynamicArray<std::string> arr;
		std::string str = "Hello";
		arr.PushBack(std::move(str));

		assert(arr.Size() == 1);
		assert(arr[0] == "Hello");
		assert(str.empty());  // Original should be empty after move
	}

	// EmplaceBack()
	{
		DynamicArray<std::string> arr;
		arr.EmplaceBack("Hello");
		arr.EmplaceBack(5, 'A');  // "AAAAA"

		assert(arr.Size() == 2);
		assert(arr[0] == "Hello");
		assert(arr[1] == "AAAAA");
	}

	// PopBack()
	{
		DynamicArray<int> arr{ 1, 2, 3, 4, 5 };
		size_t capacityBefore = arr.Capacity();
		arr.PopBack();

		assert(arr.Size() == 4);
		assert(arr.Back() == 4);
		assert(arr.Capacity() == capacityBefore); // Capacity unchanged
	}

	// Resize() (size reduction)
	{
		DynamicArray<int> arr{ 1, 2, 3, 4, 5 };
		size_t capacityBefore = arr.Capacity();
		arr.Resize(3);

		assert(arr.Size() == 3);
		assert(arr[0] == 1);
		assert(arr[2] == 3);
		assert(arr.Capacity() == capacityBefore); // Capacity unchanged
	}

	// Resize() (size increase, default value)
	{
		DynamicArray<int> arr{ 1, 2, 3 };
		arr.Resize(5);

		assert(arr.Size() == 5);
		assert(arr[0] == 1);
		assert(arr[2] == 3);
		// New elements should be default initialized (0 for int)
		assert(arr[3] == 0);
		assert(arr[4] == 0);
	}

	// Resize() (size increase, specific value)
	{
		DynamicArray<int> arr{ 1, 2, 3 };
		arr.Resize(5, 99);

		assert(arr.Size() == 5);
		assert(arr[0] == 1);
		assert(arr[2] == 3);
		assert(arr[3] == 99);
		assert(arr[4] == 99);
	}

	// Swap()
	{
		DynamicArray<int> a{ 1, 2, 3 };
		DynamicArray<int> b{ 4, 5, 6, 7 };

		size_t aSize = a.Size();
		size_t aCapacity = a.Capacity();
		size_t bSize = b.Size();
		size_t bCapacity = b.Capacity();

		a.Swap(b);

		assert(a.Size() == bSize);
		assert(a.Capacity() == bCapacity);
		assert(b.Size() == aSize);
		assert(b.Capacity() == aCapacity);

		assert(a[0] == 4);
		assert(b[0] == 1);
	}
}

void TestComparisonOperators()
{
	// operator==
	{
		DynamicArray<int> a{ 1, 2, 3 };
		DynamicArray<int> b{ 1, 2, 3 };
		DynamicArray<int> c{ 1, 2 };
		DynamicArray<int> d{ 1, 2, 4 };

		assert(a == b);
		assert(!(a == c));
		assert(!(a == d));
	}

	// operator<=> (three-way comparison)
	{
		DynamicArray<int> a{ 1, 2, 3 };
		DynamicArray<int> b{ 1, 2, 3 };
		DynamicArray<int> c{ 1, 2 };
		DynamicArray<int> d{ 1, 2, 4 };
		DynamicArray<int> e{ 0, 9, 9 };

		assert((a <=> b) == std::strong_ordering::equal);
		assert((a <=> c) == std::strong_ordering::greater);
		assert((a <=> d) == std::strong_ordering::less);
		assert((a <=> e) == std::strong_ordering::greater);
	}

	// Relational operator
	{
		DynamicArray<int> a{ 1, 2, 3 };
		DynamicArray<int> b{ 1, 2, 4 };
		DynamicArray<int> c{ 1, 2, 3, 4 };

		assert(a < b);
		assert(a <= b);
		assert(b > a);
		assert(b >= a);
		assert(a != b);

		assert(a < c);  // Different lengths
	}
}

void TestComplexTypes()
{
	// Custom type
	{
		struct Point
		{
			int x, y;

			Point(int x, int y)
				: x(x)
				, y(y)
			{
			}

			bool operator==(const Point& other) const
			{
				return x == other.x && y == other.y;
			}
		};

		DynamicArray<Point> arr;
		arr.EmplaceBack(1, 2);
		arr.PushBack(Point(3, 4));

		assert(arr.Size() == 2);
		assert(arr[0] == Point(1, 2));
		assert(arr[1] == Point(3, 4));
	}

	// std::string
	{
		DynamicArray<std::string> arr;
		arr.EmplaceBack("Hello");
		arr.PushBack("World");
		arr.EmplaceBack(3, '!');  // "!!!"

		assert(arr.Size() == 3);
		assert(arr[0] == "Hello");
		assert(arr[1] == "World");
		assert(arr[2] == "!!!");

		arr.PopBack();
		assert(arr.Size() == 2);
		assert(arr.Back() == "World");
	}
}

int main()
{
	std::cout << "Starting DynamicArray tests\n\n";

	RunTest("Constructors", TestConstructors);
	RunTest("Assignment operators", TestAssignmentOperators);
	RunTest("Element access", TestElementAccess);
	RunTest("Capacity", TestCapacity);
	RunTest("Modifiers", TestModifiers);
	RunTest("Comparison operators", TestComparisonOperators);
	RunTest("Complex types", TestComplexTypes);

	std::cout << "\nAll tests passed!\n";

	return 0;
}
