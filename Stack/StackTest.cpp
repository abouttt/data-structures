#include <compare>
#include <iostream>
#include <string>
#include <utility>

#include "Stack.h"

#define TEST_ASSERT(expr) \
    if (!(expr)) \
        throw std::string( \
            "Assertion failed: " #expr "\n" \
            "  File: " __FILE__ "\n" \
            "  Line: " + std::to_string(__LINE__))

#define RUN_TEST(func) \
    std::cout << "[TEST] " #func " ... "; \
    try \
    { \
        func(); \
        std::cout << "PASSED\n"; \
    } \
    catch (const std::string& msg) \
    { \
        std::cout << "FAILED\n  " << msg << "\n"; \
        ++failedCount; \
    }

void TestConstructors()
{
	Stack<int> stack1;
	TEST_ASSERT(stack1.Size() == 0);

	for (int i = 0; i < 5; ++i)
	{
		stack1.Push(i + 1);
	}

	// Copy constructor must perform deep copy
	Stack<int> stack2 = stack1;
	stack1.Peek() = 50;
	TEST_ASSERT(stack2.Peek() == 5);

	// Move constructor must leave source in valid empty state
	Stack<int> stack3 = std::move(stack2);
	TEST_ASSERT(stack3.Size() == 5);
	TEST_ASSERT(stack2.Size() == 0);
}

void TestAssignments()
{
	Stack<int> stack1;
	Stack<int> stack2;

	for (int i = 0; i < 3; ++i)
	{
		stack1.Push(i + 1);
	}

	// Copy assignment must perform deep copy
	stack2 = stack1;
	stack1.Peek() = 30;
	TEST_ASSERT(stack2.Peek() == 3);

	// Move assignment must leave source in valid empty state
	Stack<int> stack3;
	stack3 = std::move(stack2);
	TEST_ASSERT(stack3.Size() == 3);
	TEST_ASSERT(stack2.Size() == 0);
}

void TestElementAccess()
{
	Stack<int> stack;

	for (int i = 0; i < 5; ++i)
	{
		stack.Push(i + 1);
	}

	TEST_ASSERT(stack.Peek() == 5);

	stack.Peek() = 50;
	TEST_ASSERT(stack.Peek() == 50);
}

void TestCapacity()
{
	Stack<int> stack;
	TEST_ASSERT(stack.IsEmpty());
}

void TestModifiers()
{
	Stack<int> stack;

	stack.Push(1);
	TEST_ASSERT(stack.Size() == 1);
	TEST_ASSERT(stack.Peek() == 1);

	stack.Push(2);
	TEST_ASSERT(stack.Size() == 2);
	TEST_ASSERT(stack.Peek() == 2);

	stack.Emplace(3);
	TEST_ASSERT(stack.Size() == 3);
	TEST_ASSERT(stack.Peek() == 3);

	stack.Pop();
	TEST_ASSERT(stack.Size() == 2);
	TEST_ASSERT(stack.Peek() == 2);

	stack.Pop();
	TEST_ASSERT(stack.Size() == 1);
	TEST_ASSERT(stack.Peek() == 1);

	stack.Clear();
	TEST_ASSERT(stack.Size() == 0);
}

void TestComparisons()
{
	Stack<int> a;
	a.Push(1);
	a.Push(2);
	a.Push(3);

	Stack<int> b;
	b.Push(1);
	b.Push(2);
	b.Push(3);

	Stack<int> c;
	c.Push(1);
	c.Push(2);
	c.Push(4);

	Stack<int> d;
	d.Push(1);
	d.Push(2);

	Stack<int> empty1;
	Stack<int> empty2;

	// operator==
	TEST_ASSERT(a == b);
	TEST_ASSERT(!(a == c));
	TEST_ASSERT(!(a == d));
	TEST_ASSERT(empty1 == empty2);

	// operator<=>
	TEST_ASSERT((a <=> b) == std::strong_ordering::equal);
	TEST_ASSERT((a <=> c) == std::strong_ordering::less);
	TEST_ASSERT((c <=> a) == std::strong_ordering::greater);
	TEST_ASSERT((a <=> d) == std::strong_ordering::greater);
	TEST_ASSERT((d <=> a) == std::strong_ordering::less);
	TEST_ASSERT((empty1 <=> empty2) == std::strong_ordering::equal);
}

int main()
{
	std::cout << "Running Stack Tests...\n\n";

	int failedCount = 0;

	RUN_TEST(TestConstructors);
	RUN_TEST(TestAssignments);
	RUN_TEST(TestElementAccess);
	RUN_TEST(TestCapacity);
	RUN_TEST(TestModifiers);
	RUN_TEST(TestComparisons);

	std::cout << "\n========================================\n\n";

	if (failedCount == 0)
	{
		std::cout << "ALL TESTS PASSED SUCCESSFULLY!\n";
	}
	else
	{
		std::cout << failedCount << " TEST(S) FAILED.\n";
	}

	return failedCount;
}
