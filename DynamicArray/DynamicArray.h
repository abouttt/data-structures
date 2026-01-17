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
	// Constants
	// =======================================================

	static constexpr std::size_t INDEX_NONE = static_cast<std::size_t>(-1);

	// =======================================================
	// Constructors / Destructor
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
		std::size_t count = init.size();
		if (count > 0)
		{
			reallocate(count);
			std::uninitialized_copy(init.begin(), init.end(), m_data);
			m_size = count;
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
		if (m_data)
		{
			destroyAndDeallocate();
		}
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
			if (m_data)
			{
				destroyAndDeallocate();
			}

			m_data = std::exchange(other.m_data, nullptr);
			m_size = std::exchange(other.m_size, 0);
			m_capacity = std::exchange(other.m_capacity, 0);
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

	T& operator[](std::size_t index)
	{
		return m_data[index];
	}

	const T& operator[](std::size_t index) const
	{
		return m_data[index];
	}

	T& At(std::size_t index)
	{
		checkRange(index);
		return m_data[index];
	}

	const T& At(std::size_t index) const
	{
		checkRange(index);
		return m_data[index];
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

	void Shrink()
	{
		if (m_size < m_capacity)
		{
			reallocate(m_size);
		}
	}

	// =======================================================
	// Modifiers
	// =======================================================

	void Add(const T& value)
	{
		Emplace(value);
	}

	void Add(T&& value)
	{
		Emplace(std::move(value));
	}

	void Add(std::initializer_list<T> ilist)
	{
		std::size_t count = ilist.size();
		if (count == 0)
		{
			return;
		}

		ensureCapacity(m_size + count);

		std::uninitialized_copy(ilist.begin(), ilist.end(), m_data + m_size);
		m_size += count;
	}

	template <typename... Args>
	T& Emplace(Args&&... args)
	{
		ensureCapacity(m_size + 1);

		T* newElement = std::construct_at(m_data + m_size, std::forward<Args>(args)...);
		++m_size;

		return *newElement;
	}

	template <typename... Args>
	T& EmplaceAt(std::size_t index, Args&&... args)
	{
		checkRange(index, true);

		ensureCapacity(m_size + 1);

		shiftRight(index, 1);
		T* newElement = std::construct_at(m_data + index, std::forward<Args>(args)...);
		++m_size;

		return *newElement;
	}

	void Insert(std::size_t index, const T& value)
	{
		EmplaceAt(index, value);
	}

	void Insert(std::size_t index, T&& value)
	{
		EmplaceAt(index, std::move(value));
	}

	void Insert(std::size_t index, std::initializer_list<T> ilist)
	{
		checkRange(index, true);

		std::size_t count = ilist.size();
		if (count == 0)
		{
			return;
		}

		ensureCapacity(m_size + count);

		shiftRight(index, count);
		std::uninitialized_copy(ilist.begin(), ilist.end(), m_data + index);
		m_size += count;
	}

	bool Remove(const T& value)
	{
		std::size_t index = Find(value);
		if (index != INDEX_NONE)
		{
			RemoveAt(index);
			return true;
		}

		return false;
	}

	void RemoveAt(std::size_t index)
	{
		checkRange(index);

		std::destroy_at(m_data + index);
		shiftLeft(index, 1);
		--m_size;
	}

	void RemoveAt(std::size_t index, std::size_t count)
	{
		checkRange(index);

		if (count == 0)
		{
			return;
		}

		if (index + count > m_size)
		{
			count = m_size - index;
		}

		std::destroy_n(m_data + index, count);
		shiftLeft(index, count);
		m_size -= count;
	}

	template <typename Predicate>
	std::size_t RemoveAll(Predicate pred)
	{
		std::size_t write = 0;

		for (std::size_t read = 0; read < m_size; ++read)
		{
			if (!pred(m_data[read]))
			{
				if (write != read)
				{
					std::construct_at(m_data + write, std::move_if_noexcept(m_data[read]));
					std::destroy_at(m_data + read);
				}

				++write;
			}
			else
			{
				std::destroy_at(m_data + read);
			}
		}

		std::size_t removed = m_size - write;
		m_size = write;

		return removed;
	}

	void Clear()
	{
		if (m_data)
		{
			std::destroy_n(m_data, m_size);
			m_size = 0;
		}
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
	// Lookup
	// =======================================================

	bool Contains(const T& value) const
	{
		return Find(value) != INDEX_NONE;
	}

	template <typename Predicate>
	bool ContainsIf(Predicate pred) const
	{
		return FindIf(pred) != INDEX_NONE;
	}

	std::size_t Find(const T& value) const
	{
		for (std::size_t i = 0; i < m_size; ++i)
		{
			if (m_data[i] == value)
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

	template <typename Predicate>
	std::size_t FindIf(Predicate pred) const
	{
		for (std::size_t i = 0; i < m_size; ++i)
		{
			if (pred(m_data[i]))
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

	std::size_t FindLast(const T& value) const
	{
		for (std::size_t i = m_size; i-- > 0;)
		{
			if (m_data[i] == value)
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

	template <typename Predicate>
	std::size_t FindLastIf(Predicate pred) const
	{
		for (std::size_t i = m_size; i-- > 0;)
		{
			if (pred(m_data[i]))
			{
				return i;
			}
		}

		return INDEX_NONE;
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

		for (std::size_t i = 0; i < lhs.m_size; ++i)
		{
			if (lhs.m_data[i] != rhs.m_data[i])
			{
				return false;
			}
		}

		return true;
	}

	friend auto operator<=>(const DynamicArray& lhs, const DynamicArray& rhs)
	{
		std::size_t minSize = lhs.m_size < rhs.m_size ? lhs.m_size : rhs.m_size;

		for (std::size_t i = 0; i < minSize; ++i)
		{
			auto cmp = lhs.m_data[i] <=> rhs.m_data[i];
			if (cmp != 0)
			{
				return cmp;
			}
		}

		return lhs.m_size <=> rhs.m_size;
	}

private:
	void checkRange(std::size_t index, bool bIncludeEnd = false) const
	{
		if (index >= m_size + (bIncludeEnd ? 1 : 0))
		{
			throw std::out_of_range("out of range");
		}
	}

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
		constexpr std::size_t max = std::numeric_limits<std::size_t>::max();
		if (m_capacity > max - m_capacity / 2)
		{
			return max;
		}

		std::size_t geometric = m_capacity + m_capacity / 2;
		if (geometric < newSize)
		{
			return newSize;
		}

		return geometric;
	}

	void shiftRight(std::size_t index, std::size_t count)
	{
		std::size_t moveCount = m_size - index;

		for (std::size_t i = moveCount; i > 0; --i)
		{
			std::size_t src = index + i - 1;
			std::size_t dest = src + count;
			std::construct_at(m_data + dest, std::move_if_noexcept(m_data[src]));
			std::destroy_at(m_data + src);
		}
	}

	void shiftLeft(std::size_t index, std::size_t count)
	{
		std::size_t moveCount = m_size - index - count;

		for (std::size_t i = 0; i < moveCount; ++i)
		{
			std::size_t src = index + count + i;
			std::size_t dest = index + i;
			std::construct_at(m_data + dest, std::move_if_noexcept(m_data[src]));
			std::destroy_at(m_data + src);
		}
	}

	void reallocate(std::size_t newCapacity)
	{
		if (newCapacity == 0)
		{
			if (m_data)
			{
				destroyAndDeallocate();
				m_data = nullptr;
				m_size = 0;
				m_capacity = 0;
			}

			return;
		}

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

			destroyAndDeallocate();
		}

		m_data = newData;
		m_size = newSize;
		m_capacity = newCapacity;
	}

	void destroyAndDeallocate()
	{
		std::destroy_n(m_data, m_size);
		::operator delete(m_data);
	}

	T* m_data;
	std::size_t m_size;
	std::size_t m_capacity;
};
