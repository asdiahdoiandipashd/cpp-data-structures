#pragma once
#include"AVLTree.h"
#include"RBTree.h"

template<class Key,class Value>
class mymap {
public:
	using key_type = Key;
	using value_type = std::pair<key_type, Value>;

	//Î±º¯Êý
	struct KeyOfT
	{
		const Key& operator()(const value_type& obj) {
			return obj.first;
		}
	};

	using Node = RBTreeNode<value_type>;
	using Tree = RBTree<key_type, value_type,KeyOfT>;
	using Iterator = typename RBTree<key_type, value_type, KeyOfT>::Iterator;

	mymap()
		:_root()
	{ }

	void Insert(const value_type& obj) {
		_root.Insert(obj);
	}

	Iterator begin() {
		return _root.begin();
	}

	Iterator end() {
		return _root.end();
	}

private:

	Tree _root;

};