#pragma once
#include <core/core.hpp>

namespace ray
{
	template<typename Type, typename IndexSize = u32>
	class Array
	{
		template<typename Type>
		class Node
		{
		public:
			Node* Next;
			Type Data;

			Node(Type data = Type(), Node* next = nullptr)
			{
				this->Data = data;
				this->Next = next;
			}
		};

		IndexSize _size;
		Node<Type>* _head;

	public:
		Array()
		{
			_size = 0;
			_head = nullptr;
		}

		~Array()
		{
			Clear();
		}

		// void Push(type data, IndexSize index);

		void PushFront(Type data)
		{
			_head = new Node<Type>(data, _head);
			_size++;
		}

		void PushBack(Type data)
		{
			if (_head == nullptr)
				_head = new Node<Type>(data);
			else
			{
				Node<Type>* current = _head;

				while (current->Next != nullptr)
					current = current->Next;

				current->Next = new Node<Type>(data);
			}

			_size++;
		}

		// void Insert(type data, IndexSize index);

		// void InsertFront(type data);
		// void InsertBack(type data);

		void Pop(IndexSize index)
		{
			if (index == 0)
				PopFront();
			else
			{
				Node<Type>* previous = _head;
				for (IndexSize i = 0; i < index - 1; i++)
					previous = previous->Next;

				Node<Type>* toDelete = previous->Next;

				previous->Next = toDelete->Next;

				delete toDelete;

				_size--;
			}
		}

		void PopFront()
		{
			Node<Type>* temp = _head;

			_head = _head->Next;

			delete temp;

			_size--;
		}

		void PopBack()
		{
			Pop(_size - 1);
		}

		void Clear()
		{
			while (_size)
				PopFront();
		}

		IndexSize Size() { return this->_size; }
		bool IsEmpty() { return this->_size == 0; }

		Type& operator[](const IndexSize index)
		{
			IndexSize counter = 0;

			Node<Type>* current = _head;

			while (current != nullptr)
			{
				if (counter == index)
					return current->Data;

				current = current->Next;
				counter++;
			}
		}

		class Iterator /* нужно переписать этот класс */
		{
			Node<Type>* _current;

		public:
			Iterator() noexcept : _current(this->_head) {}

			Iterator(const Node<Type>* unnamed) noexcept : _current(unnamed) {}

			Iterator& operator=(Node<Type>* unnamed)
			{
				this->_current = unnamed;

				return *this;
			}

			bool operator!=(const Iterator& iterator)
			{
				return this->_current != iterator._current;
			}

			Iterator& operator++()
			{
				if (this->_current)
					this->_current = this->_current->_next_node;

				return *this;
			}

			Iterator operator++(IndexSize)
			{
				Iterator iterator = *this;
				++* this;

				return iterator;
			}

			Type operator*()
			{
				return this->_current->Data;
			}
		};

		Iterator begin()
		{
			return Iterator(this->_head);
		}

		Iterator end()
		{
			return Iterator(nullptr);
		}
	};
}