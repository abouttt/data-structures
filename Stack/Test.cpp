#include <cassert>
#include <iostream>

#include "Stack.h"

int main()
{
	Stack<int> s;

	// Push
	s.Push(1);
	s.Push(2);
	s.Push(3);
	assert(s.GetSize() == 3);
	assert(!s.IsEmpty());
	assert(s.Top() == 3);

	// Pop
	s.Pop();
	assert(s.Top() == 2);
	assert(s.GetSize() == 2);

	// Copy
	Stack<int> copy(s);
	assert(copy.GetSize() == s.GetSize());
	assert(copy.Top() == s.Top());

	// Move
	Stack<int> moved(std::move(copy));
	assert(moved.GetSize() == s.GetSize());

	// Comparison
	Stack<int> a;
	a.Push(1);
	a.Push(2);
	Stack<int> b;
	b.Push(1);
	b.Push(2);
	assert(a == b);
	assert((a <=> b) == 0);

	std::cout << "All Stack tests passed!\n";
}
