#include <cassert>
#include <compare>
#include <iostream>
#include <string>
#include <utility>

#include "Queue.h"

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
		Queue<int> q;
		assert(q.Size() == 0);
		assert(q.Capacity() == 0);
	}

	// Capacity constructor
	{
		Queue<int> q(10);
		assert(q.Size() == 0);
		assert(q.Capacity() == 10);
	}

	// Initializer list constructor
	{
		Queue<int> q{ 1, 2, 3, 4, 5 };
		assert(q.Size() == 5);
		assert(q.Capacity() == 5);
		assert(q.Front() == 1);
		assert(q.Back() == 5);
	}

	// Copy constructor
	{
		Queue<int> original{ 1,2,3 };
		Queue<int> copy(original);

		assert(original.Size() == copy.Size());
		assert(original.Capacity() == copy.Capacity());
		assert(original.Front() == copy.Front());
		assert(original.Back() == copy.Back());

		// Deep copy
		original.Pop();

		assert(original.Size() != copy.Size());
		assert(original.Front() != copy.Front());
	}

	// Move constructor
	{
		Queue<int> original{ 1, 2, 3 };
		size_t originalSize = original.Size();
		size_t originalCapacity = original.Capacity();

		Queue<int> moved(std::move(original));

		assert(moved.Size() == originalSize);
		assert(moved.Capacity() == originalCapacity);
		assert(moved.Front() == 1);
		assert(moved.Back() == 3);
		assert(original.Size() == 0);
		assert(original.Capacity() == 0);
	}
}

void TestAssignmentOperators()
{
	// Copy assignment operator
	{
		Queue<int> original{ 1, 2, 3 };
		Queue<int> copy;
		copy = original;

		assert(original.Size() == copy.Size());
		assert(original.Front() == copy.Front());
		assert(original.Back() == copy.Back());

		// Deep copy
		original.Pop();

		assert(original.Size() != copy.Size());
		assert(original.Front() != copy.Front());
	}

	// Move assignment operator
	{
		Queue<int> original{ 1, 2, 3 };
		size_t originalSize = original.Size();
		size_t originalCapacity = original.Capacity();

		Queue<int> moved;
		moved = std::move(original);

		assert(moved.Size() == originalSize);
		assert(moved.Capacity() == originalCapacity);
		assert(moved.Front() == 1);
		assert(moved.Back() == 3);
		assert(original.Size() == 0);
		assert(original.Capacity() == 0);
	}

	// Initializer list assignment operator
	{
		Queue<int> q;
		q = { 1, 2, 3, 4, 5 };

		assert(q.Size() == 5);
		assert(q.Front() == 1);
		assert(q.Back() == 5);
	}
}

void TestElementAccess()
{
	Queue<int> q{ 10, 20, 30, 40, 50 };

	// Front(), Back()
	assert(q.Front() == 10);
	assert(q.Back() == 50);

	q.Front() = 100;
	q.Back() = 500;
	assert(q.Front() == 100);
	assert(q.Back() == 500);
}

void TestCapacity()
{
	// IsEmpty()
	{
		Queue<int> emptyQ;
		Queue<int> nonEmptyQ;
		nonEmptyQ.Push(1);

		assert(emptyQ.IsEmpty() == true);
		assert(nonEmptyQ.IsEmpty() == false);
	}

	// Size(), Capacity()
	{
		Queue<int> q;
		assert(q.Size() == 0);
		assert(q.Capacity() == 0);

		q.Push(1);
		assert(q.Size() == 1);
		assert(q.Capacity() >= 1);

		q.Push(2);
		q.Push(3);
		assert(q.Size() == 3);
	}

	// Reserve()
	{
		Queue<int> q;
		q.Reserve(100);
		assert(q.Capacity() == 100);
		assert(q.Size() == 0);

		// When capacity is already sufficient
		size_t oldCapacity = q.Capacity();
		q.Reserve(oldCapacity / 2);
		assert(q.Capacity() == oldCapacity);  // Capacity should not shrink
	}

	// ShrinkToFit()
	{
		Queue<int> q;
		for (int i = 0; i < 10; ++i)
		{
			q.Push(i);
		}

		size_t sizeBeforeShrink = q.Size();
		size_t capacityBeforeShrink = q.Capacity();
		q.ShrinkToFit();

		assert(q.Size() == sizeBeforeShrink);
		assert(q.Size() == q.Capacity());  // Capacity should equal size
		assert(q.Capacity() <= capacityBeforeShrink); // Should not grow
		assert(q.Front() == 0);
		assert(q.Back() == 9);
	}
}

void TestModifiers()
{
	// Clear()
	{
		Queue<int> q{ 1, 2, 3, 4, 5 };
		size_t capacityBeforeClear = q.Capacity();
		q.Clear();

		assert(q.Size() == 0);
		assert(q.Capacity() == capacityBeforeClear); // Capacity should be preserved
	}

	// Push() (const lvalue reference)
	{
		Queue<int> q;
		for (int i = 0; i < 100; ++i)
		{
			q.Push(i);
		}

		assert(q.Size() == 100);
		assert(q.Front() == 0);
		assert(q.Back() == 99);
	}

	// Push() (rvalue reference)
	{
		Queue<std::string> q;
		std::string str = "Hello";
		q.Push(std::move(str));

		assert(q.Size() == 1);
		assert(q.Front() == "Hello");
		assert(str.empty());  // Original should be empty after move
	}

	// Emplace()
	{
		Queue<std::string> q;
		q.Emplace("Hello");
		q.Emplace(5, 'A');  // "AAAAA"

		assert(q.Size() == 2);
		assert(q.Front() == "Hello");
		assert(q.Back() == "AAAAA");
	}

	// Pop()
	{
		Queue<int> q{ 1, 2, 3, 4, 5 };
		size_t capacityBefore = q.Capacity();
		q.Pop();

		assert(q.Size() == 4);
		assert(q.Front() == 2);
		assert(q.Capacity() == capacityBefore); // Capacity unchanged
	}

	// Swap()
	{
		Queue<int> a{ 1, 2, 3 };
		Queue<int> b{ 4, 5, 6, 7 };

		size_t aSize = a.Size();
		size_t aCapacity = a.Capacity();
		size_t bSize = b.Size();
		size_t bCapacity = b.Capacity();

		a.Swap(b);

		assert(a.Size() == bSize);
		assert(a.Capacity() == bCapacity);
		assert(b.Size() == aSize);
		assert(b.Capacity() == aCapacity);

		assert(a.Front() == 4);
		assert(b.Front() == 1);
		assert(a.Back() == 7);
		assert(b.Back() == 3);
	}
}

void TestComparisonOperators()
{
	// operator==
	{
		Queue<int> a{ 1, 2, 3 };
		Queue<int> b{ 1, 2, 3 };
		Queue<int> c{ 1, 2 };
		Queue<int> d{ 1, 2, 4 };

		assert(a == b);
		assert(!(a == c));
		assert(!(a == d));
	}

	// operator<=> (three-way comparison)
	{
		Queue<int> a{ 1, 2, 3 };
		Queue<int> b{ 1, 2, 3 };
		Queue<int> c{ 1, 2 };
		Queue<int> d{ 1, 2, 4 };
		Queue<int> e{ 0, 9, 9 };

		assert((a <=> b) == std::strong_ordering::equal);
		assert((a <=> c) == std::strong_ordering::greater);
		assert((a <=> d) == std::strong_ordering::less);
		assert((a <=> e) == std::strong_ordering::greater);
	}

	// Relational operators
	{
		Queue<int> a{ 1, 2, 3 };
		Queue<int> b{ 1, 2, 4 };
		Queue<int> c{ 1, 2, 3, 4 };

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

		Queue<Point> q;
		q.Emplace(1, 2);
		q.Emplace(3, 4);
		q.Emplace(5, 6);

		assert(q.Size() == 3);
		assert(q.Front() == Point(1, 2));
		assert(q.Back() == Point(5, 6));

		q.Pop();
		assert(q.Front() == Point(3, 4));
	}

	// std::string
	{
		Queue<std::string> q;
		q.Emplace("Hello");
		q.Push("World");
		q.Emplace(3, '!');  // "!!!"

		assert(q.Size() == 3);
		assert(q.Front() == "Hello");
		assert(q.Back() == "!!!");

		q.Pop();
		assert(q.Size() == 2);
		assert(q.Front() == "World");
	}
}

int main()
{
	std::cout << "Starting Queue tests\n\n";

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
