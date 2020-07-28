#pragma once
#include <core/core.hpp>

namespace ray
{
template<typename type, typename IndexSize = u32>
class Array
{
	IndexSize _size;
	Node<type>* _head;

public:
	Array()
	{
		_size = 0;
		this->_head = nullptr;
	}

	~Array()
	{
		Clear();
	}

	// void Push(type data, int index);

	void PushFront(type data)
	{
		this->_head = new Node<type>(data, this->_head);
		_size++;
	}

	void PushBack(type data)
	{
		if (this->_head == nullptr)
			this->_head = new Node<type>(data);
		else
		{
			Node<type>* current = this->_head;

			while (current->_next != nullptr)
				current = current->_next;

			current->_next = new Node<type>(data);
		}

		_size++;
	}

	// void Insert(type data, int index);

	// void InsertFront(type data);
	// void InsertBack(type data);

	void Pop(IndexSize index)
	{
		if (index == 0)
			PopFront();
		else
		{
			Node<type>* previous = this->_head;
			for (IndexSize i = 0; i < index - 1; i++)
				previous = previous->_next;

			Node<type>* toDelete = previous->_next;

			previous->_next = toDelete->_next;

			delete toDelete;

			_size--;
		}
	}

	void PopFront()
	{
		Node<type>* temp = this->_head;

		this->_head = this->_head->_next;

		delete temp;

		_size--;
	}

	void PopBack()
	{
		Pop(_size - 1);
	}

	void Clear()
	{
		while (this->_size)
			PopFront();
	}

	IndexSize Size() { return this->_size; }
	bool IsEmpty() { return this->_size == 0; }

	type& operator[](const int index)
	{
		IndexSize counter = 0;

		Node<type>* current = this->_head;

		while (current != nullptr)
		{
			if (counter == index)
				return current->_data;

			current = current->_next;
			counter++;
		}
	}

private:
	template<typename type>
	class Node
	{
	public:
		Node* _next;
		type _data;

		Node(type data = type(), Node* next = nullptr)
		{
			this->_data = data;
			this->_next = next;
		}
	};
};
}
