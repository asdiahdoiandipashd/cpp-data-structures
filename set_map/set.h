#pragma once
#include"RBTree.h"

template<class Key>
class myset {
public:
	struct KeyOfT
	{
		const Key& operator()(const Key& obj) {
			return obj;
		}
	};

	using key_type = Key;
	using value_type = Key;
	using Tree = RBTree<key_type, value_type, KeyOfT>;
	using Iterator = typename RBTree<key_type, value_type, KeyOfT>::Iterator;

	myset(const value_type& obj) 
	{}

	myset()
		:_root()
	{}

	void Insert(const value_type& kv) {
		_root.Insert(kv);
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