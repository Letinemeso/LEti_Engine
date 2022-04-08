#ifndef __TREE
#define __TREE

#include "../Debug.h"

namespace LEti {

	template<typename Stored_Type, unsigned int _childs_per_node>
	class Tree
	{
	private:
		struct Node
		{
			Stored_Type* data = nullptr;
			Node* child[_childs_per_node] = { nullptr };
			Node* parent = nullptr;

			Node(Stored_Type* _new_data) : data(_new_data) { ASSERT(_new_data == nullptr); }
			~Node() { delete data; }

			void insert_after(Stored_Type* _new_data, unsigned int _child_index);
		};

		Node* head = nullptr;

	public:
		friend class Iterator;
		class Iterator
		{
		private:
			Node** head;
			Node* node = nullptr;

		private:

		public:
			Iterator() {  }
			Iterator(Node*& _node) : node(_node), head(&_node) {  }
			Iterator(Iterator&& _other) : node(_other.node) {  }
			Iterator(const Iterator& _other) : node(_other.node) {  }
			void operator=(const Iterator& _other) { node = _other.node; }

		public:
			Stored_Type& operator*() { return *(node->data); }
			
			bool operator==(const Iterator& _other) { return node == _other.node; }
			bool operator!=(const Iterator& _other) { return !(*this == _other); }

			void insert_after(Stored_Type* _new_data, unsigned int _child_index);

			void ascend();
			void descend(unsigned int _child_index);
		};

		Iterator begin() { return Iterator(head); }
		Iterator end() { return Iterator(); }

	};



	template<typename Stored_Type, unsigned int _cpn>
	void Tree<Stored_Type, _cpn>::Node::insert_after(Stored_Type* _new_data, unsigned int _child_index)
	{
		ASSERT(_child_index >= _cpn);
		Node* pnext = child[_child_index];
		child[_child_index] = new Node(_new_data);
		child[_child_index]->parent = this;
		if (pnext)
		{
			child[_child_index]->child[0] = pnext;
			pnext->parent = child[_child_index];
		}
	}



	template<typename Stored_Type, unsigned int _cpn>
	void Tree<Stored_Type, _cpn>::Iterator::insert_after(Stored_Type* _new_data, unsigned int _child_index)
	{
		if (!*head)
		{
			*head = new Node(_new_data);
			node = *head;
		}
		else node->insert_after(_new_data, _child_index);
	}

	template<typename Stored_Type, unsigned int _cpn>
	void Tree<Stored_Type, _cpn>::Iterator::ascend()
	{
		ASSERT(!head);
		ASSERT(!node->parent);
		node = node->parent;
	}

	template<typename Stored_Type, unsigned int _cpn>
	void Tree<Stored_Type, _cpn>::Iterator::descend(unsigned int _ci)
	{
		ASSERT(!head);
		ASSERT(_ci >= _cpn);
		ASSERT(!node);
		node = node->child[_ci];
	}




} /*LEti*/

#endif