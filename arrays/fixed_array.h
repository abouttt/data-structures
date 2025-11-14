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

	constexpr reference at(size_type pos)
	{
		if (pos >= N) {
			throw std::out_of_range("FixedArray::at - index out of range");
		}
		return elems[pos];
	}

	constexpr const_reference at(size_type pos) const
	{
		if (pos >= N) {
			throw std::out_of_range("FixedArray::at - index out of range");
		}
		return elems[pos];
	}

	constexpr reference operator[](size_type pos) noexcept
	{
		return elems[pos];
	}

	constexpr const_reference operator[](size_type pos) const noexcept
	{
		return elems[pos];
	}

	constexpr reference front() noexcept
	{
		return elems[0];
	}

	constexpr const_reference front() const noexcept
	{
		return elems[0];
	}

	constexpr reference back() noexcept
	{
		return elems[N - 1];
	}

	constexpr const_reference back() const noexcept
	{
		return elems[N - 1];
	}

	constexpr T* data() noexcept
	{
		return elems;
	}

	constexpr const T* data() const noexcept
	{
		return elems;
	}

	constexpr iterator begin() noexcept
	{
		return elems;
	}

	constexpr const_iterator begin() const noexcept
	{
		return elems;
	}

	constexpr const_iterator cbegin() const noexcept
	{
		return elems;
	}

	constexpr iterator end() noexcept
	{
		return elems + N;
	}

	constexpr const_iterator end() const noexcept
	{
		return elems + N;
	}

	constexpr const_iterator cend() const noexcept
	{
		return elems + N;
	}

	constexpr reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(end());
	}

	constexpr const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(end());
	}

	constexpr const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(end());
	}

	constexpr reverse_iterator rend() noexcept
	{
		return reverse_iterator(begin());
	}

	constexpr const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(begin());
	}

	constexpr const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(begin());
	}

	constexpr bool empty() const noexcept
	{
		return N == 0;
	}

	constexpr size_type size() const noexcept
	{
		return N;
	}

	constexpr size_type max_size() const noexcept
	{
		return N;
	}

	constexpr void fill(const T& value)
	{
		std::fill_n(elems, N, value);
	}

	constexpr void swap(FixedArray& other) noexcept(std::is_nothrow_swappable_v<T>)
	{
		std::swap_ranges(elems, elems + N, other.elems);
	}

	T elems[N]; // aggregate initialization
};

template <typename T, std::size_t N>
constexpr void swap(FixedArray<T, N>& lhs, FixedArray<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
	lhs.swap(rhs);
}

template <typename T, std::size_t N>
constexpr bool operator==(const FixedArray<T, N>& lhs, const FixedArray<T, N>& rhs)
{
	return std::equal(lhs.data(), lhs.data() + N, rhs.data());
}

template <typename T, std::size_t N>
constexpr auto operator<=>(const FixedArray<T, N>& lhs, const FixedArray<T, N>& rhs)
{
	return std::lexicographical_compare_three_way(
		lhs.data(), lhs.data() + N,
		rhs.data(), rhs.data() + N
	);
}
