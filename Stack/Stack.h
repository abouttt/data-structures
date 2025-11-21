#pragma once

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <memory>
#include <type_traits>
#include <utility>

template <typename T>
class Stack
{
public:
	// --- Constructor / Destructor / operator= ---  

	Stack()
		: m_Data(nullptr)
		, m_Size(0)
		, m_Capacity(0)
	{
	}

	explicit Stack(size_t capacity)
		: Stack()
	{
		if (capacity > 0)
		{
			allocate(capacity);
		}
	}

	Stack(std::initializer_list<T> init)
		: Stack()
	{
		if (init.size() > 0)
		{
			allocate(init.size());
			std::uninitialized_copy(init.begin(), init.end(), m_Data);
			m_Size = init.size();
		}
	}

	Stack(const Stack& other)
		: Stack()
	{
		if (other.m_Size > 0)
		{
			allocate(other.m_Size);
			std::uninitialized_copy_n(other.m_Data, other.m_Size, m_Data);
			m_Size = other.m_Size;
		}
	}

	Stack(Stack&& other) noexcept
		: m_Data(std::exchange(other.m_Data, nullptr))
		, m_Size(std::exchange(other.m_Size, 0))
		, m_Capacity(std::exchange(other.m_Capacity, 0))
	{
	}

	~Stack()
	{
		deallocate();
	}

	Stack& operator=(const Stack& other)
	{
		if (this != &other)
		{
			Stack temp(other);
			Swap(temp);
		}
		return *this;
	}

	Stack& operator=(Stack&& other) noexcept
	{
		if (this != &other)
		{
			Stack temp(std::move(other));
			Swap(temp);
		}
		return *this;
	}

	// --- Element access ---

	T& Top()
	{
		assert(m_Size > 0 && "Top() called on empty Stack");
		return m_Data[m_Size - 1];
	}

	const T& Top() const
	{
		assert(m_Size > 0 && "Top() called on empty Stack");
		return m_Data[m_Size - 1];
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

	// --- Modifiers ---

	void Push(const T& value)
	{
		Emplace(value);
	}

	void Push(T&& value)
	{
		Emplace(std::move(value));
	}

	template <typename... Args>
	T& Emplace(Args&&... args)
	{
		ensureCapacity(m_Size + 1);
		T* newElement = std::construct_at(m_Data + m_Size, std::forward<Args>(args)...);
		++m_Size;
		return *newElement;
	}

	void Pop()
	{
		assert(m_Size > 0 && "Pop() called on empty Stack");
		std::destroy_at(m_Data + m_Size - 1);
		--m_Size;
	}

	void Swap(Stack& other) noexcept
	{
		std::swap(m_Data, other.m_Data);
		std::swap(m_Size, other.m_Size);
		std::swap(m_Capacity, other.m_Capacity);
	}

	// --- Comparison operators ---

	template <typename U>
	friend bool operator==(const Stack<U>& lhs, const Stack<U>& rhs)
	{
		if (lhs.m_Size != rhs.m_Size)
		{
			return false;
		}
		return std::equal(lhs.m_Data, lhs.m_Data + lhs.m_Size, rhs.m_Data);
	}

	template <typename U>
	friend auto operator<=>(const Stack<U>& lhs, const Stack<U>& rhs)
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

	T* m_Data;
	size_t m_Size;
	size_t m_Capacity;
};
