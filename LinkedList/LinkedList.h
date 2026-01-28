#pragma once

#include <initializer_list>
#include <utility>

template <typename T>
class LinkedList
{
public:
	class Node
	{
	private:
		friend class LinkedList;

	public:
		T& Value()
		{
			return m_value;
		}

		const T& Value() const
		{
			return m_value;
		}

		Node* Next()
		{
			return m_next;
		}

		const Node* Next() const
		{
			return m_next;
		}

		Node* Prev()
		{
			return m_prev;
		}

		const Node* Prev() const
		{
			return m_prev;
		}

	private:
		template <typename... Args>
		Node(Args&&... args)
			: m_value(std::forward<Args>(args)...)
			, m_next(nullptr)
			, m_prev(nullptr)
		{
		}

		T m_value;
		Node* m_next;
		Node* m_prev;
	};

	LinkedList()
		: m_head(nullptr)
		, m_tail(nullptr)
		, m_size(0)
	{
	}

	explicit LinkedList(std::size_t count)
		: LinkedList(count, T{})
	{
	}

	LinkedList(std::size_t count, const T& value)
		: LinkedList()
	{
		for (std::size_t i = 0; i < count; ++i)
		{
			AddTail(value);
		}
	}

	LinkedList(std::initializer_list<T> init)
		: LinkedList()
	{
		for (const T& value : init)
		{
			AddTail(value);
		}
	}

	LinkedList(const LinkedList& other)
		: LinkedList()
	{
		Node* current = other.m_head;
		while (current)
		{
			AddTail(current->m_value);
			current = current->m_next;
		}
	}

	LinkedList(LinkedList&& other) noexcept
		: m_head(std::exchange(other.m_head, nullptr))
		, m_tail(std::exchange(other.m_tail, nullptr))
		, m_size(std::exchange(other.m_size, 0))
	{
	}

	~LinkedList()
	{
		Clear();
	}

	LinkedList& operator=(const LinkedList& other)
	{
		if (this != std::addressof(other))
		{
			LinkedList temp(other);
			Swap(temp);
		}

		return *this;
	}

	LinkedList& operator=(LinkedList&& other) noexcept
	{
		if (this != std::addressof(other))
		{
			Clear();

			m_head = std::exchange(other.m_head, nullptr);
			m_tail = std::exchange(other.m_tail, nullptr);
			m_size = std::exchange(other.m_size, 0);
		}

		return *this;
	}

	LinkedList& operator=(std::initializer_list<T> ilist)
	{
		LinkedList temp(ilist);
		Swap(temp);

		return *this;
	}

	Node* Head()
	{
		return m_head;
	}

	const Node* Head() const
	{
		return m_head;
	}

	Node* Tail()
	{
		return m_tail;
	}

	const Node* Tail() const
	{
		return m_tail;
	}

	bool IsEmpty() const
	{
		return m_size == 0;
	}

	std::size_t Size() const
	{
		return m_size;
	}

	void AddHead(const T& value)
	{
		EmplaceHead(value);
	}

	void AddHead(T&& value)
	{
		EmplaceHead(std::move(value));
	}

	void AddTail(const T& value)
	{
		EmplaceTail(value);
	}

	void AddTail(T&& value)
	{
		EmplaceTail(std::move(value));
	}

	template <typename... Args>
	Node* Emplace(Node* before, Args&&... args)
	{
		Node* newNode = new Node(std::forward<Args>(args)...);

		if (m_size == 0)
		{
			m_head = m_tail = newNode;
		}
		else if (before == m_head)
		{
			newNode->m_next = m_head;
			m_head->m_prev = newNode;
			m_head = newNode;
		}
		else if (before == nullptr)
		{
			newNode->m_prev = m_tail;
			m_tail->m_next = newNode;
			m_tail = newNode;
		}
		else
		{
			Node* prev = before->m_prev;
			newNode->m_next = before;
			newNode->m_prev = prev;
			prev->m_next = newNode;
			before->m_prev = newNode;
		}

		++m_size;

		return newNode;
	}

	template <typename... Args>
	Node* EmplaceHead(Args&&... args)
	{
		return Emplace(m_head, std::forward<Args>(args)...);
	}

	template <typename... Args>
	Node* EmplaceTail(Args&&... args)
	{
		return Emplace(nullptr, std::forward<Args>(args)...);
	}

	void Insert(Node* before, const T& value)
	{
		Emplace(before, value);
	}

	void Insert(Node* before, T&& value)
	{
		Emplace(before, std::move(value));
	}

	bool Remove(const T& value)
	{
		Node* node = Find(value);
		if (node)
		{
			Remove(node);
			return true;
		}

		return false;
	}

	void Remove(Node* node)
	{
		if (!node)
		{
			return;
		}

		Node* prev = node->m_prev;
		Node* next = node->m_next;

		if (prev)
		{
			prev->m_next = next;
		}
		else
		{
			m_head = next;
		}

		if (next)
		{
			next->m_prev = prev;
		}
		else
		{
			m_tail = prev;
		}

		delete node;
		--m_size;
	}

	void RemoveHead()
	{
		Remove(m_head);
	}

	void RemoveTail()
	{
		Remove(m_tail);
	}

	void Clear()
	{
		Node* current = m_head;
		while (current)
		{
			Node* next = current->m_next;
			delete current;
			current = next;
		}

		m_head = nullptr;
		m_tail = nullptr;
		m_size = 0;
	}

	void Resize(std::size_t count)
	{
		Resize(count, T{});
	}

	void Resize(std::size_t count, const T& value)
	{
		if (count < m_size)
		{
			while (m_size > count)
			{
				RemoveTail();
			}
		}
		else if (count > m_size)
		{
			while (m_size < count)
			{
				AddTail(value);
			}
		}
	}

	void Swap(LinkedList& other) noexcept
	{
		std::swap(m_head, other.m_head);
		std::swap(m_tail, other.m_tail);
		std::swap(m_size, other.m_size);
	}

	bool Contains(const T& value) const
	{
		return Find(value) != nullptr;
	}

	Node* Find(const T& value)
	{
		return const_cast<Node*>(static_cast<const LinkedList&>(*this).Find(value));
	}

	const Node* Find(const T& value) const
	{
		Node* current = m_head;
		while (current)
		{
			if (current->m_value == value)
			{
				return current;
			}

			current = current->m_next;
		}

		return nullptr;
	}

	Node* FindLast(const T& value)
	{
		return const_cast<Node*>(static_cast<const LinkedList&>(*this).FindLast(value));
	}

	const Node* FindLast(const T& value) const
	{
		Node* current = m_tail;
		while (current)
		{
			if (current->m_value == value)
			{
				return current;
			}

			current = current->m_prev;
		}

		return nullptr;
	}

	friend bool operator==(const LinkedList& lhs, const LinkedList& rhs)
	{
		if (lhs.m_size != rhs.m_size)
		{
			return false;
		}

		Node* lhsCurrent = lhs.m_head;
		Node* rhsCurrent = rhs.m_head;

		while (lhsCurrent && rhsCurrent)
		{
			if (lhsCurrent->Value() != rhsCurrent->Value())
			{
				return false;
			}

			lhsCurrent = lhsCurrent->Next();
			rhsCurrent = rhsCurrent->Next();
		}

		return true;
	}

	friend auto operator<=>(const LinkedList& lhs, const LinkedList& rhs)
	{
		Node* lhsCurrent = lhs.m_head;
		Node* rhsCurrent = rhs.m_head;

		while (lhsCurrent && rhsCurrent)
		{
			auto cmp = lhsCurrent->Value() <=> rhsCurrent->Value();
			if (cmp != 0)
			{
				return cmp;
			}

			lhsCurrent = lhsCurrent->Next();
			rhsCurrent = rhsCurrent->Next();
		}

		return lhs.m_size <=> rhs.m_size;
	}

private:
	Node* m_head;
	Node* m_tail;
	std::size_t m_size;
};
