#include <cassert>
#include <iostream>

#include "LinkedList.h"

int main()
{
	LinkedList<int> list;

	// PushBack test
	list.PushBack(1);
	list.PushBack(2);
	list.PushBack(3);
	assert(list.GetSize() == 3);
	assert(list.Front() == 1);
	assert(list.Back() == 3);

	// PushFront test
	list.PushFront(0);
	assert(list.GetSize() == 4);
	assert(list.Front() == 0);
	assert(list.Back() == 3);

	// PopBack test
	list.PopBack();
	assert(list.GetSize() == 3);
	assert(list.Back() == 2);

	// PopFront test
	list.PopFront();
	assert(list.GetSize() == 2);
	assert(list.Front() == 1);
	assert(list.Back() == 2);

	// Resize larger
	list.Resize(5, 10);
	assert(list.GetSize() == 5);
	assert(list.Back() == 10);

	// Resize smaller
	list.Resize(2);
	assert(list.GetSize() == 2);
	assert(list.Front() == 1);
	assert(list.Back() == 2);

	// Copy constructor
	LinkedList<int> copy(list);
	assert(copy.GetSize() == list.GetSize());
	assert(copy.Front() == list.Front());
	assert(copy.Back() == list.Back());

	// Move constructor
	LinkedList<int> moved(std::move(copy));
	assert(moved.GetSize() == 2);
	assert(moved.Front() == 1);
	assert(moved.Back() == 2);

	// Clear test
	moved.Clear();
	assert(moved.IsEmpty());

	std::cout << "All LinkedList tests passed!\n";
}
