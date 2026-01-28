#pragma once

#include <algorithm>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>

template <typename T>
class Stack
{
public:
	Stack()
		: m_data(nullptr)
		, m_size(0)
		, m_capacity(0)
	{
	}

	Stack(const Stack& other)
		: Stack()
	{
		if (other.m_size > 0)
		{
			m_data = allocate(other.m_size);
			m_capacity = other.m_size;
			std::uninitialized_copy_n(other.m_data, other.m_size, m_data);
			m_size = other.m_size;
		}
	}

	Stack(Stack&& other) noexcept
		: m_data(std::exchange(other.m_data, nullptr))
		, m_size(std::exchange(other.m_size, 0))
		, m_capacity(std::exchange(other.m_capacity, 0))
	{
	}

	~Stack()
	{
		std::destroy_n(m_data, m_size);
		deallocate(m_data);
	}

	Stack& operator=(const Stack& other)
	{
		if (this != std::addressof(other))
		{
			Stack temp(other);
			Swap(temp);
		}

		return *this;
	}

	Stack& operator=(Stack&& other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::destroy_n(m_data, m_size);
			deallocate(m_data);

			m_data = std::exchange(other.m_data, nullptr);
			m_size = std::exchange(other.m_size, 0);
			m_capacity = std::exchange(other.m_capacity, 0);
		}

		return *this;
	}

	T& Peek()
	{
		return m_data[m_size - 1];
	}

	const T& Peek() const
	{
		return m_data[m_size - 1];
	}

	bool IsEmpty() const
	{
		return m_size == 0;
	}

	std::size_t Size() const
	{
		return m_size;
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
	void Emplace(Args&&... args)
	{
		ensureCapacity(m_size + 1);

		std::construct_at(m_data + m_size, std::forward<Args>(args)...);
		++m_size;
	}

	void Pop()
	{
		--m_size;
		std::destroy_at(m_data + m_size);
	}

	void Clear()
	{
		std::destroy_n(m_data, m_size);
		m_size = 0;
	}

	void Swap(Stack& other) noexcept
	{
		std::swap(m_data, other.m_data);
		std::swap(m_size, other.m_size);
		std::swap(m_capacity, other.m_capacity);
	}

	friend bool operator==(const Stack& lhs, const Stack& rhs)
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

	friend auto operator<=>(const Stack& lhs, const Stack& rhs)
	{
		std::size_t minSize = std::min(lhs.m_size, rhs.m_size);

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
	void ensureCapacity(std::size_t minCapacity)
	{
		if (minCapacity <= m_capacity)
		{
			return;
		}

		std::size_t newCapacity = m_capacity + m_capacity / 2;

		if (m_capacity > std::numeric_limits<std::size_t>::max() - m_capacity / 2)
		{
			newCapacity = std::numeric_limits<std::size_t>::max();
		}

		if (newCapacity < minCapacity)
		{
			newCapacity = minCapacity;
		}

		reallocate(newCapacity);
	}

	T* allocate(std::size_t capacity)
	{
		if (capacity == 0)
		{
			return nullptr;
		}

		return static_cast<T*>(::operator new(capacity * sizeof(T)));
	}

	void deallocate(T* ptr)
	{
		if (ptr)
		{
			::operator delete(ptr);
		}
	}

	void reallocate(std::size_t newCapacity)
	{
		T* newData = allocate(newCapacity);
		std::size_t newSize = std::min(m_size, newCapacity);

		if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>)
		{
			std::uninitialized_move_n(m_data, newSize, newData);
		}
		else
		{
			std::uninitialized_copy_n(m_data, newSize, newData);
		}

		std::destroy_n(m_data, m_size);
		deallocate(m_data);

		m_data = newData;
		m_size = newSize;
		m_capacity = newCapacity;
	}

	T* m_data;
	std::size_t m_size;
	std::size_t m_capacity;
};
