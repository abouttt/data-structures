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
	// Constructors / Destructor
	// =======================================================

	DynamicArray()
		: m_Data(nullptr)
		, m_Size(0)
		, m_Capacity(0)
	{
	}

	explicit DynamicArray(size_t capacity)
		: DynamicArray()
	{
		if (capacity > 0)
		{
			allocate(capacity);
		}
	}

	DynamicArray(std::initializer_list<T> init)
		: DynamicArray()
	{
		if (init.size() > 0)
		{
			allocate(init.size());
			std::uninitialized_copy(init.begin(), init.end(), m_Data);
			m_Size = init.size();
		}
	}

	DynamicArray(const DynamicArray& other)
		: DynamicArray()
	{
		if (other.m_Size > 0)
		{
			allocate(other.m_Size);
			std::uninitialized_copy_n(other.m_Data, other.m_Size, m_Data);
			m_Size = other.m_Size;
		}
	}

	DynamicArray(DynamicArray&& other) noexcept
		: m_Data(std::exchange(other.m_Data, nullptr))
		, m_Size(std::exchange(other.m_Size, 0))
		, m_Capacity(std::exchange(other.m_Capacity, 0))
	{
	}

	~DynamicArray()
	{
		deallocate();
	}

	// =======================================================
	// Assignment operators
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

	DynamicArray& operator=(std::initializer_list<T> init)
	{
		DynamicArray temp(init);
		Swap(temp);
		return *this;
	}

	// =======================================================
	// Element access
	// =======================================================

	T& At(size_t index)
	{
		if (index >= m_Size)
		{
			throw std::out_of_range("Index out of range");
		}
		return m_Data[index];
	}

	const T& At(size_t index) const
	{
		if (index >= m_Size)
		{
			throw std::out_of_range("Index out of range");
		}
		return m_Data[index];
	}

	T& operator[](size_t index)
	{
		return m_Data[index];
	}

	const T& operator[](size_t index) const
	{
		return m_Data[index];
	}

	T& Front()
	{
		return m_Data[0];
	}

	const T& Front() const
	{
		return m_Data[0];
	}

	T& Back()
	{
		return m_Data[m_Size - 1];
	}

	const T& Back() const
	{
		return m_Data[m_Size - 1];
	}

	T* Data()
	{
		return m_Data;
	}

	const T* Data() const
	{
		return m_Data;
	}

	// =======================================================
	// Capacity
	// =======================================================

	bool IsEmpty() const
	{
		return m_Size == 0;
	}

	size_t Size() const
	{
		return m_Size;
	}

	size_t Capacity() const
	{
		return m_Capacity;
	}

	void Reserve(size_t newCapacity)
	{
		if (newCapacity > m_Capacity)
		{
			reallocate(newCapacity);
		}
	}

	void ShrinkToFit()
	{
		reallocate(m_Size);
	}

	// =======================================================
	// Modifiers
	// =======================================================

	void Clear()
	{
		std::destroy_n(m_Data, m_Size);
		m_Size = 0;
	}

	void PushBack(const T& value)
	{
		// Prevents dangling references when copying from own elements.
		if (&value >= m_Data && &value < m_Data + m_Size)
		{
			T temp = value;
			EmplaceBack(std::move(temp));
		}
		else
		{
			EmplaceBack(value);
		}
	}

	void PushBack(T&& value)
	{
		EmplaceBack(std::move(value));
	}

	template <typename... Args>
	T& EmplaceBack(Args&&... args)
	{
		ensureCapacity(m_Size + 1);
		T* newElement = std::construct_at(m_Data + m_Size, std::forward<Args>(args)...);
		++m_Size;
		return *newElement;
	}

	void PopBack()
	{
		--m_Size;
		std::destroy_at(m_Data + m_Size);
	}

	void Resize(size_t newSize)
	{
		if (newSize < m_Size)
		{
			std::destroy_n(m_Data + newSize, m_Size - newSize);
		}
		else if (newSize > m_Size)
		{
			ensureCapacity(newSize);

			if constexpr (std::is_scalar_v<T>)
			{
				std::uninitialized_value_construct_n(m_Data + m_Size, newSize - m_Size);
			}
			else
			{
				std::uninitialized_default_construct_n(m_Data + m_Size, newSize - m_Size);
			}
		}
		m_Size = newSize;
	}

	void Resize(size_t newSize, const T& value)
	{
		if (newSize < m_Size)
		{
			std::destroy_n(m_Data + newSize, m_Size - newSize);
		}
		else if (newSize > m_Size)
		{
			ensureCapacity(newSize);
			std::uninitialized_fill_n(m_Data + m_Size, newSize - m_Size, value);
		}
		m_Size = newSize;
	}

	void Swap(DynamicArray& other) noexcept
	{
		std::swap(m_Data, other.m_Data);
		std::swap(m_Size, other.m_Size);
		std::swap(m_Capacity, other.m_Capacity);
	}

	// =======================================================
	// Non-member functions
	// =======================================================

	friend bool operator==(const DynamicArray& lhs, const DynamicArray& rhs)
	{
		if (lhs.m_Size != rhs.m_Size)
		{
			return false;
		}
		return std::equal(lhs.m_Data, lhs.m_Data + lhs.m_Size, rhs.m_Data);
	}

	friend auto operator<=>(const DynamicArray& lhs, const DynamicArray& rhs)
	{
		return std::lexicographical_compare_three_way(
			lhs.m_Data, lhs.m_Data + lhs.m_Size,
			rhs.m_Data, rhs.m_Data + rhs.m_Size
		);
	}

private:
	void ensureCapacity(size_t minCapacity)
	{
		if (minCapacity > m_Capacity)
		{
			size_t newCapacity = calculateGrowth(minCapacity);
			reallocate(newCapacity);
		}
	}

	size_t calculateGrowth(size_t newSize) const
	{
		constexpr size_t max = std::numeric_limits<size_t>::max();
		if (m_Capacity > max - (m_Capacity / 2))
		{
			return max;
		}

		size_t newCapacity = m_Capacity + (m_Capacity / 2);
		if (newCapacity < newSize)
		{
			newCapacity = newSize;
		}

		return newCapacity;
	}

	void allocate(size_t capacity)
	{
		m_Data = static_cast<T*>(::operator new(capacity * sizeof(T)));
		m_Capacity = capacity;
	}

	void reallocate(size_t newCapacity)
	{
		if (newCapacity == m_Capacity)
		{
			return;
		}

		if (newCapacity == 0)
		{
			deallocate();
			return;
		}

		T* newData = static_cast<T*>(::operator new(newCapacity * sizeof(T)));
		size_t newSize = std::min(newCapacity, m_Size);

		if (m_Data)
		{
			if constexpr (std::is_nothrow_move_constructible_v<T>)
			{
				std::uninitialized_move_n(m_Data, newSize, newData);
			}
			else
			{
				std::uninitialized_copy_n(m_Data, newSize, newData);
			}

			std::destroy_n(m_Data, m_Size);
			::operator delete(m_Data);
		}

		m_Data = newData;
		m_Size = newSize;
		m_Capacity = newCapacity;
	}

	void deallocate()
	{
		if (m_Data)
		{
			std::destroy_n(m_Data, m_Size);
			::operator delete(m_Data);
			m_Data = nullptr;
			m_Size = 0;
			m_Capacity = 0;
		}
	}

	T* m_Data;
	size_t m_Size;
	size_t m_Capacity;
};
