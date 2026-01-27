#include <compare>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "DynamicArray.h"

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
	DynamicArray<int> arr1;
	TEST_ASSERT(arr1.Size() == 0);
	TEST_ASSERT(arr1.Capacity() == 0);

	DynamicArray<int> arr2(5);
	TEST_ASSERT(arr2.Size() == 5);
	TEST_ASSERT(arr2[0] == 0);

	DynamicArray<int> arr3(5, 999);
	TEST_ASSERT(arr3.Size() == 5);
	TEST_ASSERT(arr3[0] == 999);

	DynamicArray<int> arr4{ 1, 2, 3, 4, 5 };
	TEST_ASSERT(arr4.Size() == 5);
	TEST_ASSERT(arr4[0] == 1);
	TEST_ASSERT(arr4[4] == 5);

	// Copy constructor must perform deep copy
	DynamicArray<int> arr5 = arr4;
	arr4[0] = 10;
	TEST_ASSERT(arr5[0] == 1);

	// Move constructor must leave source in valid empty state
	DynamicArray<int> arr6 = std::move(arr5);
	TEST_ASSERT(arr6.Size() == 5);
	TEST_ASSERT(arr6.Data() != nullptr);
	TEST_ASSERT(arr5.Size() == 0);
	TEST_ASSERT(arr5.Data() == nullptr);
}

void TestAssignments()
{
	DynamicArray<int> arr1{ 1, 2, 3 };
	DynamicArray<int> arr2;

	// Copy assignment must perform deep copy
	arr2 = arr1;
	arr1[0] = 10;
	TEST_ASSERT(arr2[0] == 1);

	// Move assignment must leave source in valid empty state
	DynamicArray<int> arr3;
	arr3 = std::move(arr2);
	TEST_ASSERT(arr3.Size() == 3);
	TEST_ASSERT(arr3.Data() != nullptr);
	TEST_ASSERT(arr2.Size() == 0);
	TEST_ASSERT(arr2.Data() == nullptr);

	arr2 = { 4, 5, 6 };
	TEST_ASSERT(arr2.Size() == 3);
	TEST_ASSERT(arr2[0] == 4);
	TEST_ASSERT(arr2[2] == 6);
}

void TestElementAccess()
{
	DynamicArray<int> arr{ 1, 2, 3, 4, 5 };

	TEST_ASSERT(arr[0] == 1);
	TEST_ASSERT(arr[4] == 5);

	arr[0] = 10;
	TEST_ASSERT(arr[0] == 10);

	// operator[] must throw on out-of-range access
	bool caught = false;
	try
	{
		arr[5];
	}
	catch (const std::out_of_range&)
	{
		caught = true;
	}
	TEST_ASSERT(caught);

	int* data = arr.Data();
	data[4] = 50;
	TEST_ASSERT(arr[4] == 50);
}

void TestCapacity()
{
	DynamicArray<int> arr;
	TEST_ASSERT(arr.IsEmpty());

	arr.Reserve(10);
	arr.Reserve(5);
	TEST_ASSERT(arr.Capacity() >= 10);

	for (int i = 0; i < 5; ++i)
	{
		arr.Add(i);
	}
	TEST_ASSERT(arr.Size() == 5);

	arr.Shrink();
	TEST_ASSERT(arr.Capacity() == arr.Size());
}

void TestModifiers()
{
	DynamicArray<int> arr;

	arr.Add(1);									// { 1 }
	arr.AddRange({ 2, 3 });						// { 1, 2, 3 }
	TEST_ASSERT(arr.Size() == 3);

	arr.Emplace(4);								// { 1, 2, 3, 4 }
	arr.EmplaceAt(0, 0);						// { 0, 1, 2, 3, 4 }
	TEST_ASSERT(arr.Size() == 5);

	arr.Insert(2, 99);							// { 0, 1, 99, 2, 3, 4 }
	arr.InsertRange(6, { 5, 6 });				// { 0, 1, 99, 2, 3, 4, 5, 6 }
	TEST_ASSERT(arr.Size() == 8);

	bool removed = arr.Remove(99);				// { 0, 1, 2, 3, 4, 5, 6 }
	TEST_ASSERT(removed);
	TEST_ASSERT(arr.Size() == 7);

	arr.RemoveAt(0);							// { 1, 2, 3, 4, 5, 6 }
	TEST_ASSERT(arr.Size() == 6);

	arr.RemoveRange(3, 3);						// { 1, 2, 3 }
	TEST_ASSERT(arr.Size() == 3);

	arr.RemoveAll([](int v) { return v > 2; });	// { 1, 2 }
	TEST_ASSERT(arr.Size() == 2);

	arr.Resize(5, 100);							// { 1, 2, 100, 100, 100 }
	TEST_ASSERT(arr.Size() == 5);
	TEST_ASSERT(arr[2] == 100);

	arr.Clear();
	TEST_ASSERT(arr.Size() == 0);
}

void TestLookup()
{
	DynamicArray<int> arr = { 1, 2, 3, 2, 1 };

	TEST_ASSERT(arr.Contains(3));
	TEST_ASSERT(!arr.Contains(99));
	TEST_ASSERT(arr.ContainsIf([](int val) { return val == 3; }));
	TEST_ASSERT(!arr.ContainsIf([](int val) { return val == 99; }));

	TEST_ASSERT(arr.Find(2) == 1);
	TEST_ASSERT(arr.Find(99) == DynamicArray<int>::INDEX_NONE);
	TEST_ASSERT(arr.FindIf([](int val) { return val > 2; }) == 2);
	TEST_ASSERT(arr.FindIf([](int val) { return val == 99; }) == DynamicArray<int>::INDEX_NONE);

	TEST_ASSERT(arr.FindLast(2) == 3);
	TEST_ASSERT(arr.FindLast(99) == DynamicArray<int>::INDEX_NONE);
	TEST_ASSERT(arr.FindLastIf([](int val) { return val > 2; }) == 2);
	TEST_ASSERT(arr.FindLastIf([](int val) { return val == 99; }) == DynamicArray<int>::INDEX_NONE);
}

void TestComparisons()
{
	DynamicArray<int> a{ 1, 2, 3 };
	DynamicArray<int> b{ 1, 2, 3 };
	DynamicArray<int> c{ 1, 2, 4 };
	DynamicArray<int> d{ 1, 2 };
	DynamicArray<int> empty1;
	DynamicArray<int> empty2;

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
	std::cout << "Running DynamicArray Tests...\n\n";

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
