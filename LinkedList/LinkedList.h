#pragma once

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <utility>

template <typename T>
class LinkedList
{
private:
	struct NodeBase
	{
		NodeBase* Next;
		NodeBase* Prev;
	};

	struct Node : public NodeBase
	{
		T Data;
	};

public:
	// --- Constructor / Destructor / operator= ---  

	LinkedList()
		: m_Head(nullptr)
		, m_Tail(nullptr)
		, m_Size(0)
	{
	}

	LinkedList(std::initializer_list<T> init)
		: LinkedList()
	{
		if (init.size() > 0)
		{
			for (const T& value : init)
			{
				PushBack(value);
			}
		}
	}

	LinkedList(const LinkedList& other)
		: LinkedList()
	{
		if (other.m_Size > 0)
		{
			NodeBase* current = other.m_Head;
			while (current)
			{
				PushBack(static_cast<Node*>(current)->Data);
				current = current->Next;
			}
		}
	}

	LinkedList(LinkedList&& other) noexcept
		: m_Head(std::exchange(other.m_Head, nullptr))
		, m_Tail(std::exchange(other.m_Tail, nullptr))
		, m_Size(std::exchange(other.m_Size, 0))
	{
	}

	~LinkedList()
	{
		Clear();
	}

	LinkedList& operator=(const LinkedList& other)
	{
		if (this != &other)
		{
			LinkedList temp(other);
			Swap(temp);
		}
		return *this;
	}

	LinkedList& operator=(LinkedList&& other) noexcept
	{
		if (this != &other)
		{
			LinkedList temp(std::move(other));
			Swap(temp);
		}
		return *this;
	}

	// --- Element access ---

	T& Front()
	{
		assert(m_Head != nullptr && "Front() called on empty LinkedList");
		return static_cast<Node*>(m_Head)->Data;
	}

	const T& Front() const
	{
		assert(m_Head != nullptr && "Front() called on empty LinkedList");
		return static_cast<Node*>(m_Head)->Data;
	}

	T& Back()
	{
		assert(m_Tail != nullptr && "Back() called on empty LinkedList");
		return static_cast<Node*>(m_Tail)->Data;
	}

	const T& Back() const
	{
		assert(m_Tail != nullptr && "Back() called on empty LinkedList");
		return static_cast<Node*>(m_Tail)->Data;
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

	void Clear()
	{
		while (m_Head)
		{
			NodeBase* temp = m_Head;
			m_Head = m_Head->Next;
			delete static_cast<Node*>(temp);
		}
		m_Tail = nullptr;
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
		Node* newNode = new Node{ nullptr, m_Tail, T(std::forward<Args>(args)...) };
		if (m_Tail)
		{
			m_Tail->Next = newNode;
		}
		else
		{
			m_Head = newNode;
		}
		m_Tail = newNode;
		++m_Size;

		return newNode->Data;
	}

	void PopBack()
	{
		assert(m_Tail != nullptr && "PopBack() called on empty LinkedList");

		NodeBase* temp = m_Tail;
		m_Tail = m_Tail->Prev;
		if (m_Tail)
		{
			m_Tail->Next = nullptr;
		}
		else
		{
			m_Head = nullptr;
		}
		delete static_cast<Node*>(temp);
		--m_Size;
	}

	void PushFront(const T& value)
	{
		EmplaceFront(value);
	}

	void PushFront(T&& value)
	{
		EmplaceFront(std::move(value));
	}

	template <typename... Args>
	T& EmplaceFront(Args&&... args)
	{
		Node* newNode = new Node{ m_Head, nullptr, T(std::forward<Args>(args)...) };
		if (m_Head)
		{
			m_Head->Prev = newNode;
		}
		else
		{
			m_Tail = newNode;
		}
		m_Head = newNode;
		++m_Size;

		return newNode->Data;
	}

	void PopFront()
	{
		assert(m_Head != nullptr && "PopFront() called on empty LinkedList");

		NodeBase* temp = m_Head;
		m_Head = m_Head->Next;
		if (m_Head)
		{
			m_Head->Prev = nullptr;
		}
		else
		{
			m_Tail = nullptr;
		}
		delete static_cast<Node*>(temp);
		--m_Size;
	}

	void Resize(size_t count)
	{
		Resize(count, T{});
	}

	void Resize(size_t count, const T& value)
	{
		if (count < m_Size)
		{
			while (m_Size > count)
			{
				PopBack();
			}
		}
		else if (count > m_Size)
		{
			while (m_Size < count)
			{
				PushBack(value);
			}
		}
	}

	void Swap(LinkedList& other) noexcept
	{
		std::swap(m_Head, other.m_Head);
		std::swap(m_Tail, other.m_Tail);
		std::swap(m_Size, other.m_Size);
	}

	// --- Comparison operators ---

	template <typename U>
	friend bool operator==(const LinkedList<U>& lhs, const LinkedList<U>& rhs)
	{
		if (lhs.m_Size != rhs.m_Size)
		{
			return false;
		}

		NodeBase* currentLhs = lhs.m_Head;
		NodeBase* currentRhs = rhs.m_Head;
		while (currentLhs && currentRhs)
		{
			if (static_cast<Node*>(currentLhs)->Data != static_cast<Node*>(currentRhs)->Data)
			{
				return false;
			}
			currentLhs = currentLhs->Next;
			currentRhs = currentRhs->Next;
		}

		return true;
	}

	template <typename U>
	friend auto operator<=>(const LinkedList<U>& lhs, const LinkedList<U>& rhs)
	{
		NodeBase* currentLhs = lhs.m_Head;
		NodeBase* currentRhs = rhs.m_Head;
		while (currentLhs && currentRhs)
		{
			if (auto comp = static_cast<Node*>(currentLhs)->Data <=> static_cast<Node*>(currentRhs)->Data; comp != 0)
			{
				return comp;
			}
			currentLhs = currentLhs->Next;
			currentRhs = currentRhs->Next;
		}

		if (!currentLhs && !currentRhs)
		{
			return std::strong_ordering::equal;
		}
		else if (!currentLhs)
		{
			return std::strong_ordering::less;
		}
		else
		{
			return std::strong_ordering::greater;
		}
	}

private:
	NodeBase* m_Head;
	NodeBase* m_Tail;
	size_t m_Size;
};
