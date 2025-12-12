#include <cassert>
#include <compare>
#include <iostream>
#include <string>
#include <utility>

#include "LinkedList.h"

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
		LinkedList<int> list;
		assert(list.Size() == 0);
	}

	// Initializer list constructor
	{
		LinkedList<int> list{ 1, 2, 3, 4, 5 };
		assert(list.Size() == 5);
		assert(list.Front() == 1);
		assert(list.Back() == 5);
	}

	// Copy constructor
	{
		LinkedList<int> original{ 1, 2, 3 };
		LinkedList<int> copy(original);

		assert(original.Size() == copy.Size());
		assert(original.Front() == copy.Front());
		assert(original.Back() == copy.Back());
		assert(original == copy);
	}

	// Move constructor
	{
		LinkedList<int> original{ 1, 2, 3 };
		size_t originalSize = original.Size();
		int originalFront = original.Front();
		int originalBack = original.Back();

		LinkedList<int> moved(std::move(original));

		assert(moved.Size() == originalSize);
		assert(moved.Front() == originalFront);
		assert(moved.Back() == originalBack);
		assert(original.Size() == 0);
		assert(original.IsEmpty());
	}
}

void TestAssignmentOperators()
{
	// Copy assignment
	{
		LinkedList<int> original{ 1, 2, 3 };
		LinkedList<int> copy;
		copy = original;

		assert(original.Size() == copy.Size());
		assert(original.Front() == copy.Front());
		assert(original.Back() == copy.Back());
		assert(original == copy);
	}

	// Move assignment
	{
		LinkedList<int> original{ 1, 2, 3 };
		size_t originalSize = original.Size();
		int originalFront = original.Front();
		int originalBack = original.Back();

		LinkedList<int> moved;
		moved = std::move(original);

		assert(moved.Size() == originalSize);
		assert(moved.Front() == originalFront);
		assert(moved.Back() == originalBack);
		assert(original.Size() == 0);
	}

	// Initializer list assignment
	{
		LinkedList<int> list;
		list = { 1, 2, 3, 4, 5 };

		assert(list.Size() == 5);
		assert(list.Front() == 1);
		assert(list.Back() == 5);
	}
}

void TestElementAccess()
{
	LinkedList<int> list{ 10, 20, 30, 40, 50 };

	// Front(), Back()
	assert(list.Front() == 10);
	assert(list.Back() == 50);

	list.Front() = 100;
	list.Back() = 500;
	assert(list.Front() == 100);
	assert(list.Back() == 500);
}

void TestCapacity()
{
	// IsEmpty()
	{
		LinkedList<int> emptyList;
		LinkedList<int> nonEmptyList{ 1 };

		assert(emptyList.IsEmpty() == true);
		assert(nonEmptyList.IsEmpty() == false);
	}

	// Size()
	{
		LinkedList<int> list;
		assert(list.Size() == 0);

		list.PushBack(1);
		assert(list.Size() == 1);

		list.PushBack(2);
		list.PushBack(3);
		assert(list.Size() == 3);

		list.PopBack();
		assert(list.Size() == 2);
	}
}

void TestModifiers()
{
	// Clear()
	{
		LinkedList<int> list{ 1, 2, 3, 4, 5 };
		list.Clear();
		assert(list.Size() == 0);
	}

	// PushFront(), PopFront()
	{
		LinkedList<int> list;

		list.PushFront(3);
		list.PushFront(2);
		list.PushFront(1);

		assert(list.Size() == 3);
		assert(list.Front() == 1);
		assert(list.Back() == 3);

		list.PopFront();
		assert(list.Size() == 2);
		assert(list.Front() == 2);

		list.PopFront();
		assert(list.Front() == 3);

		list.PopFront();
		assert(list.IsEmpty());
	}

	// PushFront() with rvalue
	{
		LinkedList<std::string> list;
		std::string str = "Hello";
		list.PushFront(std::move(str));

		assert(list.Size() == 1);
		assert(list.Front() == "Hello");
		assert(str.empty());  // Original should be empty after move
	}

	// EmplaceFront()
	{
		LinkedList<std::string> list;
		list.EmplaceFront("World");
		list.EmplaceFront("Hello");

		assert(list.Size() == 2);
		assert(list.Front() == "Hello");
		assert(list.Back() == "World");
	}

	// PushBack(), PopBack()
	{
		LinkedList<int> list;

		list.PushBack(1);
		list.PushBack(2);
		list.PushBack(3);

		assert(list.Size() == 3);
		assert(list.Front() == 1);
		assert(list.Back() == 3);

		list.PopBack();
		assert(list.Size() == 2);
		assert(list.Back() == 2);

		list.PopBack();
		assert(list.Back() == 1);

		list.PopBack();
		assert(list.IsEmpty());
	}

	// PushBack() with rvalue
	{
		LinkedList<std::string> list;
		std::string str = "Hello";
		list.PushBack(std::move(str));

		assert(list.Size() == 1);
		assert(list.Front() == "Hello");
		assert(str.empty());  // Original should be empty after move
	}

	// EmplaceBack()
	{
		LinkedList<std::string> list;
		list.EmplaceBack("Hello");
		list.EmplaceBack(5, 'A');  // "AAAAA"

		assert(list.Size() == 2);
		assert(list.Front() == "Hello");
		assert(list.Back() == "AAAAA");
	}

	// Resize() - reduction
	{
		LinkedList<int> list{ 1, 2, 3, 4, 5 };
		list.Resize(3);
		assert(list.Size() == 3);
		assert(list.Front() == 1);
		assert(list.Back() == 3);
	}

	// Resize() - expansion with default value
	{
		LinkedList<int> list{ 1, 2, 3 };
		list.Resize(5);
		assert(list.Size() == 5);
		assert(list.Front() == 1);
		assert(list.Back() == 0);  // Default value for int
	}

	// Resize() - expansion with specific value
	{
		LinkedList<int> list{ 1, 2, 3 };
		list.Resize(5, 99);
		assert(list.Size() == 5);
		assert(list.Front() == 1);
		assert(list.Back() == 99);
	}

	// Swap()
	{
		LinkedList<int> a{ 1, 2, 3 };
		LinkedList<int> b{ 4, 5, 6, 7 };

		size_t list1Size = a.Size();
		size_t list2Size = b.Size();

		a.Swap(b);

		assert(a.Size() == list2Size);
		assert(b.Size() == list1Size);

		assert(a.Front() == 4);
		assert(a.Back() == 7);
		assert(b.Front() == 1);
		assert(b.Back() == 3);
	}
}

void TestComparisonOperators()
{
	// operator==
	{
		LinkedList<int> a{ 1, 2, 3 };
		LinkedList<int> b{ 1, 2, 3 };
		LinkedList<int> c{ 1, 2 };
		LinkedList<int> d{ 1, 2, 4 };

		assert(a == b);
		assert(!(a == c));
		assert(!(a == d));
	}

	// operator<=>
	{
		LinkedList<int> a{ 1, 2, 3 };
		LinkedList<int> b{ 1, 2, 3 };
		LinkedList<int> c{ 1, 2 };
		LinkedList<int> d{ 1, 2, 4 };
		LinkedList<int> e{ 0, 9, 9 };

		assert((a <=> b) == std::strong_ordering::equal);
		assert((a <=> c) == std::strong_ordering::greater);
		assert((a <=> d) == std::strong_ordering::less);
		assert((a <=> e) == std::strong_ordering::greater);
	}

	// Relational operators
	{
		LinkedList<int> a{ 1, 2, 3 };
		LinkedList<int> b{ 1, 2, 4 };
		LinkedList<int> c{ 1, 2, 3, 4 };

		assert(a < b);
		assert(a <= b);
		assert(b > a);
		assert(b >= a);
		assert(a != b);

		assert(a < c);  // Different lengths
	}
}

// =======================================================
// Test Complex Types
// =======================================================
void TestComplexTypes()
{
	// Custom type
	{
		struct Point
		{
			int x, y;

			Point(int x = 0, int y = 0)
				: x(x)
				, y(y)
			{
			}

			bool operator==(const Point& other) const
			{
				return x == other.x && y == other.y;
			}
		};

		LinkedList<Point> list;
		list.EmplaceBack(1, 2);
		list.EmplaceBack(3, 4);
		list.EmplaceBack(5, 6);

		assert(list.Size() == 3);
		assert(list.Front() == Point(1, 2));
		assert(list.Back() == Point(5, 6));

		list.PopFront();
		assert(list.Front() == Point(3, 4));
	}

	// std::string
	{
		LinkedList<std::string> list;
		list.EmplaceBack("Hello");
		list.PushBack("World");
		list.EmplaceBack(3, '!');  // "!!!"

		assert(list.Size() == 3);
		assert(list.Front() == "Hello");
		assert(list.Back() == "!!!");

		list.PopBack();
		assert(list.Size() == 2);
		assert(list.Back() == "World");
	}
}

int main()
{
	std::cout << "Starting LinkedList tests\n\n";

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
