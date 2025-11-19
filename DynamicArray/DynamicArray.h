#pragma once

#include <algorithm>
#include <cassert>
#include <compare>
#include <initializer_list>
#include <memory>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename T>
class DynamicArray
{
public:
	// --- Constructor / Destructor / operator= ---  

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

	// --- Element access ---

	T& operator[](size_t index)
	{
		assert(index < m_Size && "operator[] index out of range");
		return m_Data[index];
	}

	const T& operator[](size_t index) const
	{
		assert(index < m_Size && "operator[] index out of range");
		return m_Data[index];
	}

	T& At(size_t index)
	{
		if (index >= m_Size)
		{
			throw std::out_of_range("DynamicArray::At - index out of range");
		}
		return m_Data[index];
	}

	const T& At(size_t index) const
	{
		if (index >= m_Size)
		{
			throw std::out_of_range("DynamicArray::At - index out of range");
		}
		return m_Data[index];
	}

	T& Front()
	{
		assert(m_Size > 0 && "Front() called on empty DynamicArray");
		return m_Data[0];
	}

	const T& Front() const
	{
		assert(m_Size > 0 && "Front() called on empty DynamicArray");
		return m_Data[0];
	}

	T& Back()
	{
		assert(m_Size > 0 && "Back() called on empty DynamicArray");
		return m_Data[m_Size - 1];
	}

	const T& Back() const
	{
		assert(m_Size > 0 && "Back() called on empty DynamicArray");
		return m_Data[m_Size - 1];
	}

	T* GetData()
	{
		return m_Data;
	}

	const T* GetData() const
	{
		return m_Data;
	}

	// --- Capacity ---

	bool IsEmpty() const
	{
		return m_Size == 0;
	}

	size_t GetSize() const
	{
		return m_Size;
	}

	size_t GetCapacity() const
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
		if (m_Capacity > m_Size)
		{
			reallocate(m_Size);
		}
	}

	// --- Modifiers ---

	void Clear()
	{
		std::destroy_n(m_Data, m_Size);
		m_Size = 0;
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
		ensureCapacity(m_Size + 1);
		T* newElement = std::construct_at(m_Data + m_Size, std::forward<Args>(args)...);
		++m_Size;
		return *newElement;
	}

	void PopBack()
	{
		assert(m_Size > 0 && "PopBack() called on empty DynamicArray");
		std::destroy_at(m_Data + m_Size - 1);
		--m_Size;
	}

	void Resize(size_t count)
	{
		if (count < m_Size)
		{
			std::destroy_n(m_Data + count, m_Size - count);
		}
		else if (count > m_Size)
		{
			ensureCapacity(count);
			std::uninitialized_default_construct_n(m_Data + m_Size, count - m_Size);
		}
		m_Size = count;
	}

	void Resize(size_t count, const T& value)
	{
		if (count < m_Size)
		{
			std::destroy_n(m_Data + count, m_Size - count);
		}
		else if (count > m_Size)
		{
			ensureCapacity(count);
			std::uninitialized_fill_n(m_Data + m_Size, count - m_Size, value);
		}
		m_Size = count;
	}

	void Swap(DynamicArray& other) noexcept
	{
		std::swap(m_Data, other.m_Data);
		std::swap(m_Size, other.m_Size);
		std::swap(m_Capacity, other.m_Capacity);
	}

	// --- Comparison operators ---

	template <typename U>
	friend bool operator==(const DynamicArray<U>& lhs, const DynamicArray<U>& rhs)
	{
		if (lhs.m_Size != rhs.m_Size)
		{
			return false;
		}
		return std::equal(lhs.m_Data, lhs.m_Data + lhs.m_Size, rhs.m_Data);
	}

	template <typename U>
	friend auto operator<=>(const DynamicArray<U>& lhs, const DynamicArray<U>& rhs)
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
			size_t newCapacity = (m_Capacity == 0 ? 4 : m_Capacity * 2);
			if (newCapacity < minCapacity)
			{
				newCapacity = minCapacity;
			}
			reallocate(newCapacity);
		}
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
			m_Data = nullptr;
			m_Size = 0;
			m_Capacity = 0;
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
		}
	}

private:
	T* m_Data;
	size_t m_Size;
	size_t m_Capacity;
};
