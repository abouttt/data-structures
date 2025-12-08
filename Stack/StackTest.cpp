#include <cassert>
#include <compare>
#include <iostream>
#include <string>
#include <utility>

#include "Stack.h"

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
		Stack<int> stack;
		assert(stack.Size() == 0);
		assert(stack.Capacity() == 0);
	}

	// Capacity constructor
	{
		Stack<int> stack(10);
		assert(stack.Size() == 0);
		assert(stack.Capacity() == 10);
	}

	// Initializer list constructor
	{
		Stack<int> stack{ 1, 2, 3, 4, 5 };
		assert(stack.Size() == 5);
		assert(stack.Capacity() == 5);
		assert(stack.Top() == 5);  // Last element should be on top
	}

	// Copy constructor
	{
		Stack<int> original{ 1, 2, 3 };
		Stack<int> copy(original);

		assert(original.Size() == copy.Size());
		assert(original.Capacity() == copy.Capacity());
		assert(original.Top() == copy.Top());

		original.Pop();

		// Deep copy
		assert(original.Size() != copy.Size());
		assert(original.Top() != copy.Top());
	}

	// Move constructor
	{
		Stack<int> original{ 1, 2, 3 };
		size_t originalSize = original.Size();
		size_t originalCapacity = original.Capacity();

		Stack<int> moved(std::move(original));

		assert(moved.Size() == originalSize);
		assert(moved.Capacity() == originalCapacity);
		assert(moved.Top() == 3);
		assert(original.Size() == 0);
		assert(original.Capacity() == 0);
	}
}

void TestAssignmentOperators()
{
	// Copy assignment operator
	{
		Stack<int> original{ 1, 2, 3 };
		Stack<int> copy;
		copy = original;

		assert(original.Size() == copy.Size());
		assert(original.Top() == copy.Top());

		original.Pop();

		// Deep copy
		assert(original.Size() != copy.Size());
		assert(original.Top() != copy.Top());
	}

	// Move assignment operator
	{
		Stack<int> original{ 1, 2, 3 };
		size_t originalSize = original.Size();
		size_t originalCapacity = original.Capacity();

		Stack<int> moved;
		moved = std::move(original);

		assert(moved.Size() == originalSize);
		assert(moved.Capacity() == originalCapacity);
		assert(moved.Top() == 3);
		assert(original.Size() == 0);
		assert(original.Capacity() == 0);
	}

	// Initializer list assignment operator
	{
		Stack<int> stack;
		stack = { 1, 2, 3, 4, 5 };

		assert(stack.Size() == 5);
		assert(stack.Top() == 5);
	}
}

void TestElementAccess()
{
	Stack<int> stack{ 10, 20, 30, 40, 50 };

	// Top()
	assert(stack.Top() == 50);

	stack.Top() = 500;
	assert(stack.Top() == 500);
}

void TestCapacity()
{
	// IsEmpty()
	{
		Stack<int> emptyStack;
		Stack<int> nonEmptyStack{ 1 };

		assert(emptyStack.IsEmpty() == true);
		assert(nonEmptyStack.IsEmpty() == false);
	}

	// Size(), Capacity()
	{
		Stack<int> stack;
		assert(stack.Size() == 0);
		assert(stack.Capacity() == 0);

		stack.Push(1);
		assert(stack.Size() == 1);
		assert(stack.Capacity() >= 1);

		stack.Push(2);
		stack.Push(3);
		assert(stack.Size() == 3);
	}

	// Reserve()
	{
		Stack<int> stack;
		stack.Reserve(100);
		assert(stack.Capacity() == 100);
		assert(stack.Size() == 0);

		// When capacity is already sufficient
		size_t oldCapacity = stack.Capacity();
		stack.Reserve(oldCapacity / 2);
		assert(stack.Capacity() == oldCapacity);  // Capacity should not shrink
	}

	// ShrinkToFit()
	{
		Stack<int> stack;
		for (int i = 0; i < 10; ++i)
		{
			stack.Push(i);
		}

		size_t sizeBeforeShrink = stack.Size();
		size_t capacityBeforeShrink = stack.Capacity();
		stack.ShrinkToFit();

		assert(stack.Size() == sizeBeforeShrink);
		assert(stack.Size() == stack.Capacity());  // Capacity should equal size
		assert(stack.Capacity() < capacityBeforeShrink); // Should not grow
		assert(stack.Top() == 9);
	}
}

void TestModifiers()
{
	// Clear()
	{
		Stack<int> stack{ 1, 2, 3, 4, 5 };
		size_t capacityBeforeClear = stack.Capacity();
		stack.Clear();
		assert(stack.Size() == 0);
		assert(stack.Capacity() == capacityBeforeClear); // Capacity should be preserved
	}

	// Push() (const lvalue reference)
	{
		Stack<int> stack;
		for (int i = 0; i < 100; ++i)
		{
			stack.Push(i);
		}

		assert(stack.Size() == 100);
		assert(stack.Top() == 99);
	}

	// Push() (rvalue reference)
	{
		Stack<std::string> stack;
		std::string str = "Hello";
		stack.Push(std::move(str));

		assert(stack.Size() == 1);
		assert(stack.Top() == "Hello");
		assert(str.empty());  // Original should be empty after move
	}

	// Emplace()
	{
		Stack<std::string> stack;
		stack.Emplace("Hello");
		stack.Emplace(5, 'A');  // "AAAAA"

		assert(stack.Size() == 2);
		assert(stack.Top() == "AAAAA");
	}

	// Pop()
	{
		Stack<int> stack{ 1, 2, 3, 4, 5 };
		size_t capacityBefore = stack.Capacity();
		stack.Pop();

		assert(stack.Size() == 4);
		assert(stack.Top() == 4);
		assert(stack.Capacity() == capacityBefore); // Capacity unchanged
	}

	// Swap()
	{
		Stack<int> a{ 1, 2, 3 };
		Stack<int> b{ 4, 5, 6, 7 };

		size_t aSize = a.Size();
		size_t aCapacity = a.Capacity();
		size_t bSize = b.Size();
		size_t bCapacity = b.Capacity();

		a.Swap(b);

		assert(a.Size() == bSize);
		assert(a.Capacity() == bCapacity);
		assert(b.Size() == aSize);
		assert(b.Capacity() == aCapacity);

		assert(a.Top() == 7);
		assert(b.Top() == 3);
	}
}

void TestComparisonOperators()
{
	// operator==
	{
		Stack<int> a{ 1, 2, 3 };
		Stack<int> b{ 1, 2, 3 };
		Stack<int> c{ 1, 2 };
		Stack<int> d{ 1, 2, 4 };

		assert(a == b);
		assert(!(a == c));
		assert(!(a == d));
	}

	// operator<=> (three-way comparison)
	{
		Stack<int> a{ 1, 2, 3 };
		Stack<int> b{ 1, 2, 3 };
		Stack<int> c{ 1, 2 };
		Stack<int> d{ 1, 2, 4 };
		Stack<int> e{ 0, 9, 9 };

		assert((a <=> b) == std::strong_ordering::equal);
		assert((a <=> c) == std::strong_ordering::greater);
		assert((a <=> d) == std::strong_ordering::less);
		assert((a <=> e) == std::strong_ordering::greater);
	}

	// Relational operators
	{
		Stack<int> a{ 1, 2, 3 };
		Stack<int> b{ 1, 2, 4 };
		Stack<int> c{ 1, 2, 3, 4 };

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

		Stack<Point> stack;
		stack.Emplace(1, 2);
		stack.Emplace(3, 4);
		stack.Emplace(5, 6);

		assert(stack.Size() == 3);
		assert(stack.Top() == Point(5, 6));

		stack.Pop();
		assert(stack.Top() == Point(3, 4));
	}

	// std::string
	{
		Stack<std::string> stack;
		stack.Emplace("Hello");
		stack.Push("World");
		stack.Emplace(3, '!');  // "!!!"

		assert(stack.Size() == 3);
		assert(stack.Top() == "!!!");

		stack.Pop();
		assert(stack.Size() == 2);
		assert(stack.Top() == "World");
	}
}

int main()
{
	std::cout << "Starting Stack tests\n\n";

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
