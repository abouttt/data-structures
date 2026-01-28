#include <compare>
#include <iostream>
#include <string>
#include <utility>

#include "LinkedList.h"

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
	LinkedList<int> list1;
	TEST_ASSERT(list1.Size() == 0);
	TEST_ASSERT(list1.Head() == nullptr);

	LinkedList<int> list2(5);
	TEST_ASSERT(list2.Size() == 5);
	TEST_ASSERT(list2.Head()->Value() == 0);

	LinkedList<int> list3(5, 999);
	TEST_ASSERT(list3.Size() == 5);
	TEST_ASSERT(list3.Head()->Value() == 999);

	LinkedList<int> list4 = { 1, 2, 3, 4, 5 };
	TEST_ASSERT(list4.Size() == 5);
	TEST_ASSERT(list4.Head()->Value() == 1);
	TEST_ASSERT(list4.Tail()->Value() == 5);

	// Copy constructor must perform deep copy
	LinkedList<int> list5 = list4;
	list4.Head()->Value() = 10;
	TEST_ASSERT(list5.Head()->Value() == 1);

	// Move constructor must leave source in valid empty state
	LinkedList<int> list6 = std::move(list5);
	TEST_ASSERT(list6.Size() == 5);
	TEST_ASSERT(list6.Head() != nullptr);
	TEST_ASSERT(list5.Size() == 0);
	TEST_ASSERT(list5.Head() == nullptr);
}

void TestAssignments()
{
	LinkedList<int> list1 = { 1, 2, 3 };
	LinkedList<int> list2;

	// Copy assignment must perform deep copy
	list2 = list1;
	list1.Head()->Value() = 10;
	TEST_ASSERT(list2.Head()->Value() == 1);

	// Move assignment must leave source in valid empty state
	LinkedList<int> list3;
	list3 = std::move(list2);
	TEST_ASSERT(list3.Size() == 3);
	TEST_ASSERT(list3.Head() != nullptr);
	TEST_ASSERT(list2.Size() == 0);
	TEST_ASSERT(list2.Head() == nullptr);

	// Initializer-list assignment
	list2 = { 4, 5, 6 };
	TEST_ASSERT(list2.Size() == 3);
	TEST_ASSERT(list2.Head()->Value() == 4);
	TEST_ASSERT(list2.Tail()->Value() == 6);
}

void TestElementAccess()
{
	LinkedList<int> list = { 1, 2, 3, 4, 5 };

	TEST_ASSERT(list.Head()->Value() == 1);

	list.Head()->Value() = 10;
	TEST_ASSERT(list.Head()->Value() == 10);

	TEST_ASSERT(list.Tail()->Value() == 5);

	list.Tail()->Value() = 50;
	TEST_ASSERT(list.Tail()->Value() == 50);
}

void TestCapacity()
{
	LinkedList<int> list;
	TEST_ASSERT(list.IsEmpty());
}

void TestModifiers()
{
	LinkedList<int> list;

	list.AddHead(1);                    // { 1 }
	list.AddTail(2);                    // { 1, 2 }
	TEST_ASSERT(list.Size() == 2);

	list.EmplaceHead(0);                // { 0, 1, 2 }
	list.EmplaceTail(3);                // { 0, 1, 2, 3 }
	TEST_ASSERT(list.Size() == 4);

	list.Insert(list.Tail(), 99);       // { 0, 1, 2, 99, 3 }
	TEST_ASSERT(list.Size() == 5);

	bool removed = list.Remove(99);     // { 0, 1, 2, 3 }
	TEST_ASSERT(removed);
	TEST_ASSERT(list.Size() == 4);

	list.Remove(list.Head()->Next());   // { 0, 2, 3 }
	TEST_ASSERT(list.Size() == 3);

	list.RemoveHead();                  // { 2, 3 }
	TEST_ASSERT(list.Size() == 2);

	list.RemoveTail();                  // { 3 }
	TEST_ASSERT(list.Size() == 1);

	list.Resize(5, 100);                // { 3, 100, 100, 100, 100 }
	TEST_ASSERT(list.Size() == 5);
	TEST_ASSERT(list.Head()->Next()->Value() == 100);

	list.Clear();
	TEST_ASSERT(list.Size() == 0);
}

void TestLookup()
{
	LinkedList<int> list = { 1, 2, 3, 2, 1 };

	TEST_ASSERT(list.Contains(3));
	TEST_ASSERT(!list.Contains(99));

	auto node1 = list.Find(2);
	TEST_ASSERT(node1 != nullptr);
	TEST_ASSERT(node1->Value() == 2);
	TEST_ASSERT(node1->Prev()->Value() == 1);

	auto node2 = list.FindLast(2);
	TEST_ASSERT(node2 != nullptr);
	TEST_ASSERT(node2->Value() == 2);
	TEST_ASSERT(node2->Next()->Value() == 1);

	// Check validity checks
	TEST_ASSERT(node1 != node2);

	TEST_ASSERT(list.Find(99) == nullptr);
}

void TestComparisons()
{
	LinkedList<int> a = { 1, 2, 3 };
	LinkedList<int> b = { 1, 2, 3 };
	LinkedList<int> c = { 1, 2, 4 };
	LinkedList<int> d = { 1, 2 };
	LinkedList<int> empty1;
	LinkedList<int> empty2;

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
	std::cout << "Running LinkedList Tests...\n\n";

	int failedCount = 0;

	RUN_TEST(TestConstructors);
	RUN_TEST(TestAssignments);
	RUN_TEST(TestElementAccess);
	RUN_TEST(TestCapacity);
	RUN_TEST(TestModifiers);
	RUN_TEST(TestLookup);
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
