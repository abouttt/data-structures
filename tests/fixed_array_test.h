#pragma once

#include <iostream>

#include "arrays/fixed_array.h"

inline void test_fixed_array()
{
	FixedArray<int, 5> arr{};

	// size
	std::cout << "Size: " << arr.size() << "\n";

	// fill 
	arr.fill(7);

	std::cout << "Fill: ";
	for (auto v : arr) {
		std::cout << v << " ";
	}
	std::cout << "\n";

	// front/back
	std::cout << "Front: " << arr.front() << "\n";
	std::cout << "Back:  " << arr.back() << "\n";

	// swap
	FixedArray<int, 5> other{};
	other.fill(3);
	arr.swap(other);

	std::cout << "Swap (arr): ";
	for (auto v : arr) {
		std::cout << v << " ";
	}
	std::cout << "\n";

	std::cout << "Swap (other): ";
	for (auto v : other) {
		std::cout << v << " ";
	}
	std::cout << "\n";
}
