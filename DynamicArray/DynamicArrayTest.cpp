#include <compare>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "DynamicArray.h"

// =======================================================
// Test Utilities
// =======================================================

#define TEST_ASSERT(expr)                                                       \
    do                                                                          \
    {                                                                           \
        if (!(expr))                                                            \
        {                                                                       \
            std::cerr << "\n[ASSERTION FAILED]: " << #expr << "\n"              \
                      << "  File: " << __FILE__ << "\n"                         \
                      << "  Line: " << __LINE__ << std::endl;                   \
            std::abort();                                                       \
        }                                                                       \
    } while (0)

void RunTest(const std::string& name, void (*func)())
{
	std::cout << "[TEST] " << name << " ... ";
	func();
	std::cout << "PASSED" << std::endl;
}

// =======================================================
// Tests
// =======================================================

void TestConstructors()
{
	DynamicArray<int> arr1;
	TEST_ASSERT(arr1.Size() == 0);
	TEST_ASSERT(arr1.Capacity() == 0);

	DynamicArray<int> arr2(5);
	TEST_ASSERT(arr2.Size() == 5);
	TEST_ASSERT(arr2[0] == 0);

	DynamicArray<int> arr3(5, 999);
	TEST_ASSERT(arr3[0] == 999);

	DynamicArray<int> arr4{ 1, 2, 3, 4, 5 };
	TEST_ASSERT(arr4.Size() == 5);
	TEST_ASSERT(arr4[0] == 1);
	TEST_ASSERT(arr4[4] == 5);

	// Copy constructor must perform deep copy
	DynamicArray<int> arr5 = arr4;
	arr5[0] = 10;
	TEST_ASSERT(arr4[0] == 1);

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
	arr2[0] = 10;
	TEST_ASSERT(arr1[0] == 1);

	// Move assignment must leave source in valid empty state
	DynamicArray<int> arr3;
	arr3 = std::move(arr2);
	TEST_ASSERT(arr3.Size() == 3);
	TEST_ASSERT(arr2.Size() == 0);
	TEST_ASSERT(arr2.Data() == nullptr);

	// Initializer-list assignment
	arr2 = { 4, 5 };
	TEST_ASSERT(arr2.Size() == 2);
	TEST_ASSERT(arr2[1] == 5);
}

void TestElementAccess()
{
	DynamicArray<int> arr{ 1, 2, 3, 4, 5 };

	TEST_ASSERT(arr[0] == 1);
	TEST_ASSERT(arr[4] == 5);

	arr[0] = 10;
	TEST_ASSERT(arr[0] == 10);

	TEST_ASSERT(arr.At(1) == 2);

	arr.At(2) = 30;
	TEST_ASSERT(arr.At(2) == 30);

	// At() must throw on out-of-range access
	bool bCaught = false;
	try
	{
		arr.At(5);
	}
	catch (const std::out_of_range&)
	{
		bCaught = true;
	}
	TEST_ASSERT(bCaught);

	// Data() must expose internal buffer
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

	// Shrink must reduce capacity to size
	arr.Shrink();
	TEST_ASSERT(arr.Capacity() == arr.Size());
}

void TestModifiers()
{
	DynamicArray<int> arr;

	arr.Add(1);
	arr.Add({ 2, 3 });
	TEST_ASSERT(arr.Size() == 3);

	arr.Emplace(4);
	arr.EmplaceAt(0, 0);
	TEST_ASSERT(arr[0] == 0);

	arr.Insert(2, 99);
	TEST_ASSERT(arr[2] == 99);

	bool bRemoved = arr.Remove(99);
	TEST_ASSERT(bRemoved);
	TEST_ASSERT(arr.Size() == 5);

	arr.RemoveAt(0);
	TEST_ASSERT(arr[0] == 1);

	arr.RemoveAll([](int v) { return v > 2; });
	TEST_ASSERT(arr.Size() == 2);

	arr.Resize(4, 100);
	TEST_ASSERT(arr[3] == 100);

	arr.Clear();
	TEST_ASSERT(arr.Size() == 0);
}

void TestLookup()
{
	DynamicArray<std::string> arr{ "apple", "banana", "cherry" };

	// Found cases
	TEST_ASSERT(arr.Contains("banana"));
	TEST_ASSERT(arr.Find("cherry") == 2);
	TEST_ASSERT(arr.FindIf([](const std::string& s) { return s.length() == 5; }) == 0);

	// Not found cases - Should return INDEX_NONE
	TEST_ASSERT(arr.Find("durian") == DynamicArray<std::string>::INDEX_NONE);
	TEST_ASSERT(arr.FindIf([](const std::string& s) { return s.empty(); }) == DynamicArray<std::string>::INDEX_NONE);
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

// =======================================================
// Main Entry Point
// =======================================================

int main()
{
	std::cout << "Starting DynamicArray tests\n\n";

	try
	{
		RunTest("Constructors", TestConstructors);
		RunTest("Assignments", TestAssignments);
		RunTest("Element Access", TestElementAccess);
		RunTest("Capacity", TestCapacity);
		RunTest("Modifiers", TestModifiers);
		RunTest("Lookup", TestLookup);
		RunTest("Comparisons", TestComparisons);

		std::cout << "\nResult: ALL TESTS PASSED SUCCESSFULLY!" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "\n[CRITICAL ERROR]: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
