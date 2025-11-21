#include <cassert>
#include <iostream>

#include "Queue.h"

int main()
{
    Queue<int> q;

    // Push test
    q.Push(1);
    q.Push(2);
    q.Push(3);
    assert(q.GetSize() == 3);
    assert(q.Front() == 1);
    assert(q.Back() == 3);

    // Pop test
    q.Pop();
    assert(q.GetSize() == 2);
    assert(q.Front() == 2);

    // Emplace test
    q.Emplace(10);
    assert(q.Back() == 10);

    // Wrap-around test (force index wrap)
    for (int i = 0; i < 10; ++i)
    {
        q.Push(i);
    }

    assert(q.GetSize() == 13);
    assert(q.Front() == 2);       // after first pop
    assert(q.Back() == 9);

    // Copy constructor test
    Queue<int> copy(q);
    assert(copy == q);

    // Move constructor test
    Queue<int> moved(std::move(copy));
    assert(moved.GetSize() == q.GetSize());

    // Comparison operators test
    assert(moved == q);
    assert((moved <=> q) == 0);

    // Swap test
    Queue<int> s1 = { 10, 20, 30 };
    Queue<int> s2 = { 5, 6 };

    s1.Swap(s2);
    assert(s1.Front() == 5);
    assert(s1.GetSize() == 2);
    assert(s2.Front() == 10);
    assert(s2.GetSize() == 3);

    std::cout << "All Queue tests passed!\n";
}
