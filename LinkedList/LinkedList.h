#pragma once

#include <initializer_list>
#include <utility>

template <typename T>
class LinkedList
{
private:
	struct NodeBase
	{
		NodeBase* Next = nullptr;
		NodeBase* Prev = nullptr;
	};

	struct Node : public NodeBase
	{
		T Data;

		template <typename... Args>
		Node(NodeBase* prev, NodeBase* next, Args&&... args)
			: NodeBase{ next, prev }
			, Data(std::forward<Args>(args)...)
		{
		}
	};

public:
	// =======================================================
	// Constructors / Destructor
	// =======================================================

	LinkedList()
		: m_Head(nullptr)
		, m_Tail(nullptr)
		, m_Size(0)
	{
	}

	LinkedList(std::initializer_list<T> init)
		: LinkedList()
	{
		for (const T& value : init)
		{
			PushBack(value);
		}
	}

	LinkedList(const LinkedList& other)
		: LinkedList()
	{
		NodeBase* current = other.m_Head;
		while (current)
		{
			PushBack(static_cast<Node*>(current)->Data);
			current = current->Next;
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

	// =======================================================
	// Assignment operators
	// =======================================================

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

	LinkedList& operator=(std::initializer_list<T> init)
	{
		LinkedList temp(init);
		Swap(temp);
		return *this;
	}

	// =======================================================
	// Element access
	// =======================================================

	T& Front()
	{
		return static_cast<Node*>(m_Head)->Data;
	}

	const T& Front() const
	{
		return static_cast<Node*>(m_Head)->Data;
	}

	T& Back()
	{
		return static_cast<Node*>(m_Tail)->Data;
	}

	const T& Back() const
	{
		return static_cast<Node*>(m_Tail)->Data;
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

	// =======================================================
	// Modifiers
	// =======================================================

	void Clear()
	{
		NodeBase* current = m_Head;
		while (current)
		{
			NodeBase* next = current->Next;
			delete static_cast<Node*>(current);
			current = next;
		}
		m_Head = nullptr;
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
		Node* newNode = new Node(m_Tail, nullptr, std::forward<Args>(args)...);
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
		Node* newNode = new Node(nullptr, m_Head, std::forward<Args>(args)...);
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

	// =======================================================
	// Non-member functions
	// =======================================================

	friend bool operator==(const LinkedList& lhs, const LinkedList& rhs)
	{
		if (lhs.m_Size != rhs.m_Size)
		{
			return false;
		}

		NodeBase* currentLhs = lhs.m_Head;
		NodeBase* currentRhs = rhs.m_Head;
		while (currentLhs && currentRhs)
		{
			T& valLhs = static_cast<Node*>(currentLhs)->Data;
			T& valRhs = static_cast<Node*>(currentRhs)->Data;
			if (valLhs != valRhs)
			{
				return false;
			}
			currentLhs = currentLhs->Next;
			currentRhs = currentRhs->Next;
		}

		return true;
	}

	friend auto operator<=>(const LinkedList& lhs, const LinkedList& rhs)
	{
		NodeBase* currentLhs = lhs.m_Head;
		NodeBase* currentRhs = rhs.m_Head;
		while (currentLhs && currentRhs)
		{
			T& valLhs = static_cast<Node*>(currentLhs)->Data;
			T& valRhs = static_cast<Node*>(currentRhs)->Data;
			if (auto cmp = valLhs <=> valRhs; cmp != 0)
			{
				return cmp;
			}
			currentLhs = currentLhs->Next;
			currentRhs = currentRhs->Next;
		}

		return lhs.m_Size <=> rhs.m_Size;
	}

private:
	NodeBase* m_Head;
	NodeBase* m_Tail;
	size_t m_Size;
};
