#pragma once

#include <algorithm>
#include <cassert>
#include <compare>
#include <initializer_list>
#include <memory>
#include <type_traits>
#include <utility>

template <typename T>
class Queue
{
public:
	// --- Constructor / Destructor / Assignment ---

	Queue()
		: m_Data(nullptr)
		, m_Front(0)
		, m_Back(0)
		, m_Size(0)
		, m_Capacity(0)
	{
	}

	explicit Queue(size_t capacity)
		: Queue()
	{
		if (capacity > 0)
		{
			allocate(capacity);
		}
	}

	Queue(std::initializer_list<T> init)
		: Queue()
	{
		if (init.size() > 0)
		{
			allocate(init.size());
			std::uninitialized_copy(init.begin(), init.end(), m_Data);
			m_Size = init.size();
			m_Back = m_Size % m_Capacity;
		}
	}

	Queue(const Queue& other)
		: Queue()
	{
		if (other.m_Size > 0)
		{
			allocate(other.m_Size);
			copyCircular(other, m_Data);
			m_Size = other.m_Size;
			m_Back = m_Size % m_Capacity;
		}
	}

	Queue(Queue&& other) noexcept
		: m_Data(std::exchange(other.m_Data, nullptr))
		, m_Front(std::exchange(other.m_Front, 0))
		, m_Back(std::exchange(other.m_Back, 0))
		, m_Size(std::exchange(other.m_Size, 0))
		, m_Capacity(std::exchange(other.m_Capacity, 0))
	{
	}

	~Queue()
	{
		deallocateCircular();
	}

	Queue& operator=(const Queue& other)
	{
		if (this != &other)
		{
			Queue temp(other);
			Swap(temp);
		}
		return *this;
	}

	Queue& operator=(Queue&& other) noexcept
	{
		if (this != &other)
		{
			Queue temp(std::move(other));
			Swap(temp);
		}
		return *this;
	}

	// --- Element Access ---

	T& Front()
	{
		assert(m_Size > 0 && "Front() called on empty Queue");
		return m_Data[m_Front];
	}

	const T& Front() const
	{
		assert(m_Size > 0 && "Front() called on empty Queue");
		return m_Data[m_Front];
	}

	T& Back()
	{
		assert(m_Size > 0 && "Back() called on empty Queue");
		return m_Data[(m_Back + m_Capacity - 1) % m_Capacity];
	}

	const T& Back() const
	{
		assert(m_Size > 0 && "Back() called on empty Queue");
		return m_Data[(m_Back + m_Capacity - 1) % m_Capacity];
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
		T* newElement = std::construct_at(m_Data + m_Back, std::forward<Args>(args)...);
		m_Back = (m_Back + 1) % m_Capacity;
		++m_Size;
		return *newElement;
	}

	void Pop()
	{
		assert(m_Size > 0 && "Pop() called on empty Queue");
		std::destroy_at(m_Data + m_Front);
		m_Front = (m_Front + 1) % m_Capacity;
		--m_Size;
	}

	void Swap(Queue& other) noexcept
	{
		std::swap(m_Data, other.m_Data);
		std::swap(m_Front, other.m_Front);
		std::swap(m_Back, other.m_Back);
		std::swap(m_Size, other.m_Size);
		std::swap(m_Capacity, other.m_Capacity);
	}

	// --- Comparison Operators ---

	template <typename U>
	friend bool operator==(const Queue<U>& lhs, const Queue<U>& rhs)
	{
		if (lhs.m_Size != rhs.m_Size)
		{
			return false;
		}

		for (size_t i = 0; i < lhs.m_Size; ++i)
		{
			if (lhs.m_Data[(lhs.m_Front + i) % lhs.m_Capacity] !=
				rhs.m_Data[(rhs.m_Front + i) % rhs.m_Capacity])
			{
				return false;
			}
		}

		return true;
	}

	template <typename U>
	friend auto operator<=>(const Queue<U>& lhs, const Queue<U>& rhs)
	{
		size_t minSize = std::min(lhs.m_Size, rhs.m_Size);

		for (size_t i = 0; i < minSize; ++i)
		{
			auto cmp = std::compare_three_way{}(
				lhs.m_Data[(lhs.m_Front + i) % lhs.m_Capacity],
				rhs.m_Data[(rhs.m_Front + i) % rhs.m_Capacity]
				);

			if (cmp != 0)
			{
				return cmp;
			}
		}

		return std::compare_three_way{}(lhs.m_Size, rhs.m_Size);
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
			reallocateCircular(newCapacity);
		}
	}

	void copyCircular(const Queue& other, T* dest)
	{
		size_t firstChunk = std::min(other.m_Size, other.m_Capacity - other.m_Front);
		std::uninitialized_copy_n(other.m_Data + other.m_Front, firstChunk, dest);
		if (other.m_Size > firstChunk)
		{
			std::uninitialized_copy_n(other.m_Data, other.m_Size - firstChunk, dest + firstChunk);
		}
	}

	void moveCircularTo(T* dest)
	{
		size_t firstChunk = std::min(m_Size, m_Capacity - m_Front);
		size_t remain = m_Size - firstChunk;

		if constexpr (std::is_nothrow_move_constructible_v<T>)
		{
			std::uninitialized_move_n(m_Data + m_Front, firstChunk, dest);
			std::uninitialized_move_n(m_Data, remain, dest + firstChunk);
		}
		else
		{
			std::uninitialized_copy_n(m_Data + m_Front, firstChunk, dest);
			std::uninitialized_copy_n(m_Data, remain, dest + firstChunk);
		}
	}

	void allocate(size_t capacity)
	{
		m_Data = static_cast<T*>(::operator new(capacity * sizeof(T)));
		m_Capacity = capacity;
	}

	void reallocateCircular(size_t newCapacity)
	{
		T* newData = static_cast<T*>(::operator new(newCapacity * sizeof(T)));

		if (m_Data)
		{
			moveCircularTo(newData);
			deallocateCircular();
		}

		m_Data = newData;
		m_Capacity = newCapacity;
		m_Front = 0;
		m_Back = m_Size % newCapacity;
	}

	void destroyCircular()
	{
		size_t firstChunk = std::min(m_Size, m_Capacity - m_Front);
		std::destroy_n(m_Data + m_Front, firstChunk);
		if (m_Size > firstChunk)
		{
			std::destroy_n(m_Data, m_Size - firstChunk);
		}
	}

	void deallocateCircular()
	{
		if (m_Data)
		{
			destroyCircular();
			::operator delete(m_Data);
		}
	}

private:
	T* m_Data;
	size_t m_Front;
	size_t m_Back;
	size_t m_Size;
	size_t m_Capacity;
};
