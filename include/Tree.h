#ifndef __TREE
#define __TREE

#include <memory>

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

			Node(const Stored_Type& _new_data) : data(new Stored_Type(_new_data)) {  }
			Node(Stored_Type&& _new_data) : data(new Stored_Type(_new_data)) {  }
			~Node() { delete data; }

			void insert_after(const Stored_Type& _new_data, unsigned int _child_index);
			void insert_after(Stored_Type&& _new_data, unsigned int _child_index);

			static void clear_branch(Node* _node);
		};

        mutable Node* head = nullptr;

	public:
		~Tree();

	public:
		class Iterator
		{
        protected:
            Node** head;
            Node* node = nullptr;

		public:
			Iterator() {  }
            Iterator(Node** _node) : node(*_node), head(_node) {  }
            Iterator(Iterator&& _other) : node(_other.node), head(_other.head) {  }
            Iterator(const Iterator& _other) : node(_other.node), head(_other.head) {  }
            void operator=(const Iterator& _other) { node = _other.node; head = _other.head; }

        private:
			unsigned int get_child_index();

		public:
			bool valid() const;

            virtual Stored_Type& operator*() { return *(node->data); }

            bool operator==(const Iterator& _other) const { return node == _other.node; }
            bool operator!=(const Iterator& _other) const { return !(*this == _other); }

            void operator++();
            void operator--();

            virtual void insert_after(const Stored_Type& _new_data, unsigned int _child_index);
            virtual void insert_after(Stored_Type&& _new_data, unsigned int _child_index);
            virtual unsigned int insert_into_availible_index(const Stored_Type& _new_data);
            virtual unsigned int insert_into_availible_index(Stored_Type&& _new_data);

            virtual void delete_branch();

            unsigned int ascend();
			void descend(unsigned int _child_index);

			bool begin() const;
			bool end() const;
		};

        class Const_Iterator : public Iterator
        {
        private:
//            Stored_Type& operator*() override {};

            void insert_after(const Stored_Type& _new_data, unsigned int _child_index) override {};
            void insert_after(Stored_Type&& _new_data, unsigned int _child_index) override {};
            unsigned int insert_into_availible_index(const Stored_Type& _new_data) override {};
            unsigned int insert_into_availible_index(Stored_Type&& _new_data) override {};

            void delete_branch() override {};

        public:
            Const_Iterator() {  }
            Const_Iterator(Node** _node) : Iterator(_node) { }
            Const_Iterator(Iterator&& _other) : Iterator(_other) { }
            Const_Iterator(const Iterator& _other) : Iterator(_other) { }
//            void operator=(const Iterator& _other) { node = _other.node; head = _other.head; }

        public:
            Stored_Type& operator*() const { return *(Iterator::node->data); }

        };

        Iterator create_iterator() { return Iterator(&head); }
        Const_Iterator create_iterator() const { return Const_Iterator(&head); }

	public:

	};

	template<typename Stored_Type, unsigned int _cpn>
	Tree<Stored_Type, _cpn>::~Tree()
	{
		Node::clear_branch(head);
	}


	//NODE IMPLEMENTATION

	template<typename Stored_Type, unsigned int _cpn>
	void Tree<Stored_Type, _cpn>::Node::insert_after(const Stored_Type& _new_data, unsigned int _child_index)
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
	void Tree<Stored_Type, _cpn>::Node::insert_after(Stored_Type&& _new_data, unsigned int _child_index)
	{
		ASSERT(_child_index >= _cpn);
		Node* pnext = child[_child_index];
		child[_child_index] = new Node(std::move(_new_data));
		child[_child_index]->parent = this;
		if (pnext)
		{
			child[_child_index]->child[0] = pnext;
			pnext->parent = child[_child_index];
		}
	}

	template<typename Stored_Type, unsigned int _cpn>
	void Tree<Stored_Type, _cpn>::Node::clear_branch(Node* _node)
	{
		if (!_node) return;

		for (unsigned int i = 0; i < _cpn; ++i)
		{
			clear_branch(_node->child[i]);
			//delete _node->child[i];
		}
		delete _node;
	}


	//ITERATOR IMPLEMENTATION

	template<typename Stored_Type, unsigned int _cpn>
	unsigned int Tree<Stored_Type, _cpn>::Iterator::get_child_index()
	{
		ASSERT(!valid());
		if (!node->parent) return _cpn;
		for (unsigned int i = 0; i < _cpn; ++i)
			if (node->parent->child[i] == node) return i;
	}


	template<typename Stored_Type, unsigned int _cpn>
	bool Tree<Stored_Type, _cpn>::Iterator::valid() const
	{
		return *head != nullptr && node != nullptr;
	}


	template<typename Stored_Type, unsigned int _cpn>
	void Tree<Stored_Type, _cpn>::Iterator::insert_after(const Stored_Type& _new_data, unsigned int _child_index)
	{
		if (!*head)
		{
			*head = new Node(_new_data);
			node = *head;
		}
		else node->insert_after(_new_data, _child_index);
	}

	template<typename Stored_Type, unsigned int _cpn>
	void Tree<Stored_Type, _cpn>::Iterator::insert_after(Stored_Type&& _new_data, unsigned int _child_index)
	{
		if (!*head)
		{
			*head = new Node(std::move(_new_data));
			node = *head;
		}
		else
		{
			node->insert_after(std::move(_new_data), _child_index);
		}
	}

	template<typename Stored_Type, unsigned int _cpn>
	unsigned int Tree<Stored_Type, _cpn>::Iterator::insert_into_availible_index(const Stored_Type& _new_data)
	{
		if (!*head)
		{
			*head = new Node(_new_data);
			node = *head;
		}
		else
		{
			for (unsigned int i = 0; i < _cpn; ++i)
			{
				if (!node->child[i])
				{
					node->insert_after(_new_data, i);
                    return i;
				}
			}
		}
        return _cpn;
	}

	template<typename Stored_Type, unsigned int _cpn>
    unsigned int Tree<Stored_Type, _cpn>::Iterator::insert_into_availible_index(Stored_Type&& _new_data)
	{
		if (!*head)
		{
			*head = new Node(std::move(_new_data));
			node = *head;
		}
		else
		{
			for (unsigned int i = 0; i < _cpn; ++i)
			{
				if (!node->child[i])
				{
					node->insert_after(std::move(_new_data), i);
                    return i;
				}
			}
        }
        return _cpn;
	}


	template<typename Stored_Type, unsigned int _cpn>
	void Tree<Stored_Type, _cpn>::Iterator::delete_branch()
	{
		ASSERT(!*head || !node);

		unsigned int ind = get_child_index();

		if (ind == _cpn)
		{
			Node::clear_branch(*head);
			*head = nullptr;
			node = nullptr;
		}
		else
		{
			Node* pptr = node->parent;
			Node::clear_branch(pptr->child[ind]);
			pptr->child[ind] = nullptr;
		}
	}


	template<typename Stored_Type, unsigned int _cpn>
    unsigned int Tree<Stored_Type, _cpn>::Iterator::ascend()
	{
		ASSERT(!head);
		ASSERT(!node->parent);
        unsigned int prev_child_index = get_child_index();
		node = node->parent;
        return prev_child_index;
	}

	template<typename Stored_Type, unsigned int _cpn>
	void Tree<Stored_Type, _cpn>::Iterator::descend(unsigned int _ci)
	{
		ASSERT(!head);
		ASSERT(_ci >= _cpn);
		ASSERT(!node);
		ASSERT(node->child[_ci] == nullptr);
		node = node->child[_ci];
	}


	template<typename Stored_Type, unsigned int _cpn>
    void Tree<Stored_Type, _cpn>::Iterator::operator++()
	{
		ASSERT(!*head);
		ASSERT(!node);

		for (unsigned int i = 0; i < _cpn; ++i)
		{
			if (node->child[i])
			{
				node = node->child[i];
				return;
			}
		}

		Node* tptr = node;
		while (tptr)
		{
			Node* tptr_p = tptr->parent;
			if (!tptr_p) return;

			unsigned int i = 1;
			for (; i < _cpn; ++i)
				if (tptr_p->child[i - 1] == tptr) break;
			for (; i < _cpn; ++i)
			{
				if (tptr_p->child[i])
				{
					node = tptr_p->child[i];
					return;
				}
			}

			tptr = tptr_p;
		}
	}

	template<typename Stored_Type, unsigned int _cpn>
    void Tree<Stored_Type, _cpn>::Iterator::operator--()
	{
		ASSERT(!*head);
		ASSERT(!node);

		Node* tptr = node;
		Node* tptr_p = tptr->parent;
		if (!tptr_p) return;


		for (int i = get_child_index() - 1; i >= 0; --i)
		{
			if (tptr_p->child[i])
			{
				tptr = tptr_p->child[i];

				while (true)
				{
					int c = _cpn - 1;
					for (; c >= 0; --c)
					{
						if (tptr->child[c])
						{
							tptr = tptr->child[c];
							break;
						}
					}
					if (c == -1)
					{
						node = tptr;
						return;
					}
				}
			}
		}

		node = tptr_p;
	}


	template<typename Stored_Type, unsigned int _cpn>
	bool Tree<Stored_Type, _cpn>::Iterator::begin() const
	{
		ASSERT(!valid());
		return node->parent == nullptr;
	}

	template<typename Stored_Type, unsigned int _cpn>
	bool Tree<Stored_Type, _cpn>::Iterator::end() const
	{
		ASSERT(!valid());

		if (node->parent == nullptr || node->parent->child[_cpn - 1] == node)
		{
			for (unsigned int i = 0; i < _cpn; ++i)
				if (node->child[i]) return false;
		}

		for (unsigned int i = 0; i < _cpn; ++i)
			if (node->child[i]) return false;

		Node* tptr = node;
		while (tptr->parent)
		{
			if (tptr->parent->child[_cpn - 1] != tptr)
			{
				unsigned int i = 1;
				for (; i < _cpn; ++i)
					if (tptr->parent->child[i - 1] == tptr) break;
				for (; i < _cpn; ++i)
					if (tptr->parent->child[i]) return false;
			}
			tptr = tptr->parent;
		}

		return true;
	}




} /*LEti*/

#endif
