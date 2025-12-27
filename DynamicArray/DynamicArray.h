#pragma once

#include <algorithm>
#include <initializer_list>
#include <limits>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename T>
class DynamicArray
{
public:
	// =======================================================
	// Constructors / Destructors
	// =======================================================

	DynamicArray()
		: m_data(nullptr)
		, m_size(0)
		, m_capacity(0)
	{
	}

	explicit DynamicArray(std::size_t count)
		: DynamicArray()
	{
		if (count > 0)
		{
			reallocate(count);
			std::uninitialized_value_construct_n(m_data, count);
			m_size = count;
		}
	}

	DynamicArray(std::size_t count, const T& value)
		: DynamicArray()
	{
		if (count > 0)
		{
			reallocate(count);
			std::uninitialized_fill_n(m_data, count, value);
			m_size = count;
		}
	}

	DynamicArray(std::initializer_list<T> init)
		: DynamicArray()
	{
		if (init.size() > 0)
		{
			reallocate(init.size());
			std::uninitialized_copy(init.begin(), init.end(), m_data);
			m_size = init.size();
		}
	}

	DynamicArray(const DynamicArray& other)
		: DynamicArray()
	{
		if (other.m_size > 0)
		{
			reallocate(other.m_size);
			std::uninitialized_copy_n(other.m_data, other.m_size, m_data);
			m_size = other.m_size;
		}
	}

	DynamicArray(DynamicArray&& other) noexcept
		: m_data(std::exchange(other.m_data, nullptr))
		, m_size(std::exchange(other.m_size, 0))
		, m_capacity(std::exchange(other.m_capacity, 0))
	{
	}

	~DynamicArray()
	{
		std::destroy_n(m_data, m_size);
		::operator delete(m_data);
	}

	// =======================================================
	// Assignment Operators
	// =======================================================

	DynamicArray& operator=(const DynamicArray& other)
	{
		if (this != &other)
		{
			DynamicArray temp(other);
			Swap(temp);
		}
		return *this;
	}

	DynamicArray& operator=(DynamicArray&& other) noexcept
	{
		if (this != &other)
		{
			DynamicArray temp(std::move(other));
			Swap(temp);
		}
		return *this;
	}

	DynamicArray& operator=(std::initializer_list<T> ilist)
	{
		DynamicArray temp(ilist);
		Swap(temp);
		return *this;
	}

	// =======================================================
	// Element Access
	// =======================================================

	T& At(std::size_t pos)
	{
		if (pos >= m_size)
		{
			throw std::out_of_range("out of range");
		}
		return m_data[pos];
	}

	const T& At(std::size_t pos) const
	{
		if (pos >= m_size)
		{
			throw std::out_of_range("out of range");
		}
		return m_data[pos];
	}

	T& operator[](std::size_t pos)
	{
		return m_data[pos];
	}

	const T& operator[](std::size_t pos) const
	{
		return m_data[pos];
	}

	T& Front()
	{
		return m_data[0];
	}

	const T& Front() const
	{
		return m_data[0];
	}

	T& Back()
	{
		return m_data[m_size - 1];
	}

	const T& Back() const
	{
		return m_data[m_size - 1];
	}

	T* Data()
	{
		return m_data;
	}

	const T* Data() const
	{
		return m_data;
	}

	// =======================================================
	// Capacity
	// =======================================================

	bool IsEmpty() const
	{
		return m_size == 0;
	}

	std::size_t Size() const
	{
		return m_size;
	}

	std::size_t Capacity() const
	{
		return m_capacity;
	}

	void Reserve(std::size_t newCapacity)
	{
		if (newCapacity > m_capacity)
		{
			reallocate(newCapacity);
		}
	}

	void ShrinkToFit()
	{
		if (m_size < m_capacity)
		{
			reallocate(m_size);
		}
	}

	// =======================================================
	// Modifiers
	// =======================================================

	void Clear()
	{
		std::destroy_n(m_data, m_size);
		m_size = 0;
	}

	void Insert(std::size_t pos, const T& value)
	{
		Emplace(pos, value);
	}

	void Insert(std::size_t pos, T&& value)
	{
		Emplace(pos, std::move(value));
	}

	void Insert(std::size_t pos, std::size_t count, const T& value)
	{
		if (pos > m_size)
		{
			throw std::out_of_range("out of range");
		}

		if (count == 0)
		{
			return;
		}

		ensureCapacity(m_size + count);
		moveBackwards(pos, count);
		std::uninitialized_fill_n(m_data + pos, count, value);
		m_size += count;
	}

	void Insert(std::size_t pos, std::initializer_list<T> ilist)
	{
		if (pos > m_size)
		{
			throw std::out_of_range("out of range");
		}

		std::size_t count = ilist.size();
		if (count == 0)
		{
			return;
		}

		ensureCapacity(m_size + count);
		moveBackwards(pos, count);
		std::uninitialized_copy(ilist.begin(), ilist.end(), m_data + pos);
		m_size += count;
	}

	template <typename... Args>
	T& Emplace(std::size_t pos, Args&&... args)
	{
		if (pos > m_size)
		{
			throw std::out_of_range("out of range");
		}

		if (pos == m_size)
		{
			return EmplaceBack(std::forward<Args>(args)...);
		}

		ensureCapacity(m_size + 1);
		moveBackwards(pos, 1);
		T* newElement = std::construct_at(m_data + pos, std::forward<Args>(args)...);
		++m_size;

		return *newElement;
	}

	void Erase(std::size_t pos)
	{
		Erase(pos, 1);
	}

	void Erase(std::size_t pos, std::size_t count)
	{
		if (pos >= m_size)
		{
			throw std::out_of_range("out of range");
		}

		if (pos + count > m_size)
		{
			count = m_size - pos;
		}

		if (count == 0)
		{
			return;
		}

		std::destroy_n(m_data + pos, count);
		moveForwards(pos, count);
		m_size -= count;
	}

	void PushBack(const T& value)
	{
		EmplaceBack(value);
	}

	void PushBack(T&& value)
	{
		EmplaceBack(std::move(value));
	}

	template <typename... Args>
	T& EmplaceBack(Args&&... args)
	{
		ensureCapacity(m_size + 1);
		T* newElement = std::construct_at(m_data + m_size, std::forward<Args>(args)...);
		++m_size;
		return *newElement;
	}

	void PopBack()
	{
		--m_size;
		std::destroy_at(m_data + m_size);
	}

	void Resize(std::size_t newSize)
	{
		if (newSize < m_size)
		{
			std::destroy_n(m_data + newSize, m_size - newSize);
		}
		else if (newSize > m_size)
		{
			ensureCapacity(newSize);
			std::uninitialized_value_construct_n(m_data + m_size, newSize - m_size);
		}
		m_size = newSize;
	}

	void Resize(std::size_t newSize, const T& value)
	{
		if (newSize < m_size)
		{
			std::destroy_n(m_data + newSize, m_size - newSize);
		}
		else if (newSize > m_size)
		{
			ensureCapacity(newSize);
			std::uninitialized_fill_n(m_data + m_size, newSize - m_size, value);
		}
		m_size = newSize;
	}

	void Swap(DynamicArray& other) noexcept
	{
		std::swap(m_data, other.m_data);
		std::swap(m_size, other.m_size);
		std::swap(m_capacity, other.m_capacity);
	}

	// =======================================================
	// Non-member functions
	// =======================================================

	friend bool operator==(const DynamicArray& lhs, const DynamicArray& rhs)
	{
		if (lhs.m_size != rhs.m_size)
		{
			return false;
		}
		return std::equal(lhs.m_data, lhs.m_data + lhs.m_size, rhs.m_data);
	}

	friend auto operator<=>(const DynamicArray& lhs, const DynamicArray& rhs)
	{
		return std::lexicographical_compare_three_way(
			lhs.m_data, lhs.m_data + lhs.m_size,
			rhs.m_data, rhs.m_data + rhs.m_size
		);
	}

private:
	void ensureCapacity(std::size_t minCapacity)
	{
		if (minCapacity > m_capacity)
		{
			std::size_t newCapacity = calculateGrowth(minCapacity);
			reallocate(newCapacity);
		}
	}

	std::size_t calculateGrowth(std::size_t newSize) const
	{
		constexpr std::size_t maxSize = std::numeric_limits<std::size_t>::max();
		if (m_capacity > maxSize - m_capacity / 2)
		{
			return maxSize;
		}

		std::size_t newCapacity = m_capacity + m_capacity / 2;
		if (newCapacity < newSize)
		{
			newCapacity = newSize;
		}

		return newCapacity;
	}

	void moveForwards(std::size_t pos, std::size_t count)
	{
		std::size_t elementsToMove = m_size - pos - count;
		for (std::size_t i = 0; i < elementsToMove; ++i)
		{
			std::size_t src = pos + count + i;
			std::size_t dest = pos + i;
			std::construct_at(m_data + dest, std::move_if_noexcept(m_data[src]));
			std::destroy_at(m_data + src);
		}
	}

	void moveBackwards(std::size_t pos, std::size_t count)
	{
		std::size_t elementsToMove = m_size - pos;
		for (std::size_t i = elementsToMove; i > 0; --i)
		{
			std::size_t src = pos + i - 1;
			std::size_t dest = src + count;
			std::construct_at(m_data + dest, std::move_if_noexcept(m_data[src]));
			std::destroy_at(m_data + src);
		}
	}

	void reallocate(std::size_t newCapacity)
	{
		T* newData = static_cast<T*>(::operator new(newCapacity * sizeof(T)));
		std::size_t newSize = std::min(m_size, newCapacity);

		if (m_data)
		{
			if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>)
			{
				std::uninitialized_move_n(m_data, newSize, newData);
			}
			else
			{
				std::uninitialized_copy_n(m_data, newSize, newData);
			}

			std::destroy_n(m_data, m_size);
			::operator delete(m_data);
		}

		m_data = newData;
		m_size = newSize;
		m_capacity = newCapacity;
	}

	T* m_data;
	std::size_t m_size;
	std::size_t m_capacity;
};
