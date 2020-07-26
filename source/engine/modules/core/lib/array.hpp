#pragma once

namespace ray
{
	template<typename type>
	class Array
	{
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

				while (current->_nextPointer != nullptr)
					current = current->_nextPointer;

				current->_nextPointer = new Node<type>(data);
			}

			_size++;
		}

		// void Insert(type data, int index);

		// void InsertFront(type data);
		// void InsertBack(type data);

		void Pop(int index)
		{
			if (index == 0)
				PopFront();
			else
			{
				Node<type>* previous = this->_head;
				for (int i = 0; i < index - 1; i++)
					previous = previous->_nextPointer;

				Node<type>* toDelete = previous->_nextPointer;

				previous->_nextPointer = toDelete->_nextPointer;

				delete toDelete;

				_size--;
			}
		}

		void PopFront()
		{
			Node<type>* temp = this->_head;

			this->_head = this->_head->_nextPointer;

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

		int Size() { return this->_size; }
		bool IsEmpty() { return this->_size == 0; }

		type& operator[](const int index)
		{
			int counter = 0;

			Node<type>* current = this->_head;

			while (current != nullptr)
			{
				if (counter == index)
					return current->_data;

				current = current->_nextPointer;
				counter++;
			}
		}

	private:
		template<typename type>
		class Node
		{
		public:
			Node* _nextPointer;
			type _data;

			Node(type data = type(), Node* _nextPointer = nullptr)
			{
				this->_data = data;
				this->_nextPointer = _nextPointer;
			}
		};

		int _size;
		Node<type>* _head;
	};
}