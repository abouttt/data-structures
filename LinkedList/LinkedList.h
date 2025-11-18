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
		NodeBase* next;
		NodeBase* prev;
	};

	struct Node : public NodeBase
	{
		T data;
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
				PushBack(static_cast<Node*>(current)->data);
				current = current->next;
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
		return static_cast<Node*>(m_Head)->data;
	}

	const T& Front() const
	{
		assert(m_Head != nullptr && "Front() called on empty LinkedList");
		return static_cast<Node*>(m_Head)->data;
	}

	T& Back()
	{
		assert(m_Tail != nullptr && "Back() called on empty LinkedList");
		return static_cast<Node*>(m_Tail)->data;
	}

	const T& Back() const
	{
		assert(m_Tail != nullptr && "Back() called on empty LinkedList");
		return static_cast<Node*>(m_Tail)->data;
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
			m_Head = m_Head->next;
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
			m_Tail->next = newNode;
		}
		else
		{
			m_Head = newNode;
		}
		m_Tail = newNode;
		++m_Size;

		return newNode->data;
	}

	void PopBack()
	{
		assert(m_Tail != nullptr && "PopBack() called on empty LinkedList");

		NodeBase* temp = m_Tail;
		m_Tail = m_Tail->prev;
		if (m_Tail)
		{
			m_Tail->next = nullptr;
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
			m_Head->prev = newNode;
		}
		else
		{
			m_Tail = newNode;
		}
		m_Head = newNode;
		++m_Size;

		return newNode->data;
	}

	void PopFront()
	{
		assert(m_Head != nullptr && "PopFront() called on empty LinkedList");

		NodeBase* temp = m_Head;
		m_Head = m_Head->next;
		if (m_Head)
		{
			m_Head->prev = nullptr;
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

private:
	NodeBase* m_Head;
	NodeBase* m_Tail;
	size_t m_Size;
};
