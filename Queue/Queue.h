#pragma once

#include <algorithm>
#include <compare>
#include <initializer_list>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>

template <typename T>
class Queue
{
public:
	// =======================================================
	// Constructors / Destructor
	// =======================================================

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
			m_Back = m_Size;
		}
	}

	Queue(const Queue& other)
		: Queue()
	{
		if (other.m_Size > 0)
		{
			allocate(other.m_Size);
			copyCircular(other.m_Data, other.m_Front, other.m_Size, other.m_Capacity, m_Data);
			m_Size = other.m_Size;
			m_Back = m_Size;
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
		deallocate();
	}

	// =======================================================
	// Assignment operators
	// =======================================================

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

	Queue& operator=(std::initializer_list<T> init)
	{
		Queue temp(init);
		Swap(temp);
		return *this;
	}

	// =======================================================
	// Element access
	// =======================================================

	T& Front()
	{
		return m_Data[m_Front];
	}

	const T& Front() const
	{
		return m_Data[m_Front];
	}

	T& Back()
	{
		return m_Data[(m_Front + m_Size - 1) % m_Capacity];
	}

	const T& Back() const
	{
		return m_Data[(m_Front + m_Size - 1) % m_Capacity];
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
		destroyCircular();
		m_Front = 0;
		m_Back = 0;
		m_Size = 0;
	}

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
		std::destroy_at(m_Data + m_Front);
		m_Front = (m_Front + 1) % m_Capacity;
		--m_Size;

		if (m_Size == 0)
		{
			m_Front = 0;
			m_Back = 0;
		}
	}

	void Swap(Queue& other) noexcept
	{
		std::swap(m_Data, other.m_Data);
		std::swap(m_Front, other.m_Front);
		std::swap(m_Back, other.m_Back);
		std::swap(m_Size, other.m_Size);
		std::swap(m_Capacity, other.m_Capacity);
	}

	// =======================================================
	// Non-member functions
	// =======================================================

	friend bool operator==(const Queue& lhs, const Queue& rhs)
	{
		if (lhs.m_Size != rhs.m_Size)
		{
			return false;
		}

		for (size_t i = 0; i < lhs.m_Size; ++i)
		{
			size_t li = (lhs.m_Front + i) % lhs.m_Capacity;
			size_t ri = (rhs.m_Front + i) % rhs.m_Capacity;
			if (!(lhs.m_Data[li] == rhs.m_Data[ri]))
			{
				return false;
			}
		}

		return true;
	}

	friend auto operator<=>(const Queue& lhs, const Queue& rhs)
	{
		size_t minSize = std::min(lhs.m_Size, rhs.m_Size);

		for (size_t i = 0; i < minSize; ++i)
		{
			size_t li = (lhs.m_Front + i) % lhs.m_Capacity;
			size_t ri = (rhs.m_Front + i) % rhs.m_Capacity;
			auto cmp = std::compare_three_way()(lhs.m_Data[li], rhs.m_Data[ri]);
			if (cmp != 0)
			{
				return cmp;
			}
		}

		return std::compare_three_way()(lhs.m_Size, rhs.m_Size);
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

	static void copyCircular(const T* src, size_t srcFront, size_t count, size_t srcCapacity, T* dest)
	{
		if (count == 0)
		{
			return;
		}

		size_t firstPart = std::min(count, srcCapacity - srcFront);
		std::uninitialized_copy_n(src + srcFront, firstPart, dest);
		if (count > firstPart)
		{
			std::uninitialized_copy_n(src, count - firstPart, dest + firstPart);
		}
	}

	static void moveCircular(T* src, size_t srcFront, size_t count, size_t srcCapacity, T* dest)
	{
		if (count == 0)
		{
			return;
		}

		size_t firstPart = std::min(count, srcCapacity - srcFront);
		std::uninitialized_move_n(src + srcFront, firstPart, dest);
		if (count > firstPart)
		{
			std::uninitialized_move_n(src, count - firstPart, dest + firstPart);
		}
	}

	void destroyCircular()
	{
		if (!m_Data || m_Size == 0)
		{
			return;
		}

		size_t firstPart = std::min(m_Size, m_Capacity - m_Front);
		std::destroy_n(m_Data + m_Front, firstPart);

		if (m_Size > firstPart)
		{
			std::destroy_n(m_Data, m_Size - firstPart);
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
			return;
		}

		T* newData = static_cast<T*>(::operator new(newCapacity * sizeof(T)));
		size_t newSize = std::min(newCapacity, m_Size);

		if (m_Data)
		{
			if constexpr (std::is_nothrow_move_constructible_v<T>)
			{
				moveCircular(m_Data, m_Front, newSize, m_Capacity, newData);
			}
			else
			{
				copyCircular(m_Data, m_Front, newSize, m_Capacity, newData);
			}

			destroyCircular();
			::operator delete(m_Data);
		}

		m_Data = newData;
		m_Front = 0;
		m_Back = newSize % newCapacity;
		m_Size = newSize;
		m_Capacity = newCapacity;

		if (newSize == 0)
		{
			m_Back = 0;
		}
	}

	void deallocate()
	{
		if (m_Data)
		{
			destroyCircular();
			::operator delete(m_Data);
			m_Data = nullptr;
			m_Front = 0;
			m_Back = 0;
			m_Size = 0;
			m_Capacity = 0;
		}
	}

private:
	T* m_Data;
	size_t m_Front;
	size_t m_Back;
	size_t m_Size;
	size_t m_Capacity;
};
