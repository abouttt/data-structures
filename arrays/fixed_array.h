#pragma once

#include <algorithm>
#include <compare>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <type_traits>

template <typename T, std::size_t N>
class FixedArray
{
public:
	// --- Member Types ---

	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using iterator = T*;
	using const_iterator = const T*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	// --- Element Access ---

	reference at(size_type pos)
	{
		if (pos >= N) {
			throw std::out_of_range("FixedArray::at - index out of range");
		}
		return elems[pos];
	}

	const_reference at(size_type pos) const
	{
		if (pos >= N) {
			throw std::out_of_range("FixedArray::at - index out of range");
		}
		return elems[pos];
	}

	reference operator[](size_type pos)
	{
		return elems[pos];
	}

	const_reference operator[](size_type pos) const
	{
		return elems[pos];
	}

	reference front()
	{
		return elems[0];
	}

	const_reference front() const
	{
		return elems[0];
	}

	reference back()
	{
		return elems[N - 1];
	}

	const_reference back() const
	{
		return elems[N - 1];
	}

	T* data()
	{
		return elems;
	}

	const T* data() const
	{
		return elems;
	}

	// --- Iterators ---

	iterator begin()
	{
		return elems;
	}

	const_iterator begin() const
	{
		return elems;
	}

	const_iterator cbegin() const
	{
		return elems;
	}

	iterator end()
	{
		return elems + N;
	}

	const_iterator end() const
	{
		return elems + N;
	}

	const_iterator cend() const
	{
		return elems + N;
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(end());
	}

	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(end());
	}

	const_reverse_iterator crbegin() const
	{
		return const_reverse_iterator(end());
	}

	reverse_iterator rend()
	{
		return reverse_iterator(begin());
	}

	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(begin());
	}

	const_reverse_iterator crend() const
	{
		return const_reverse_iterator(begin());
	}

	// --- Capacity ---

	bool empty() const
	{
		return N == 0;
	}

	size_type size() const
	{
		return N;
	}

	size_type max_size() const
	{
		return N;
	}

	// --- Operations ---

	void fill(const T& value)
	{
		std::fill_n(elems, N, value);
	}

	void swap(FixedArray& other)
	{
		std::swap_ranges(elems, elems + N, other.elems);
	}

	T elems[N]; // aggregate initialization
};

// --- Non-member functions ---

template <typename T, std::size_t N>
bool operator==(const FixedArray<T, N>& lhs, const FixedArray<T, N>& rhs)
{
	return std::equal(lhs.data(), lhs.data() + N, rhs.data());
}

template <typename T, std::size_t N>
auto operator<=>(const FixedArray<T, N>& lhs, const FixedArray<T, N>& rhs)
{
	return std::lexicographical_compare_three_way(
		lhs.data(), lhs.data() + N,
		rhs.data(), rhs.data() + N
	);
}
