#pragma once
#include<iostream>
#include<utility>
#include<assert.h>

enum NodeColor{
	red,
	black
};

template<class T>
struct RBTreeNode
{
	using Node = RBTreeNode<T>;
	T _data;
	Node* _left;
	Node* _right;
	Node* _parent;
	NodeColor _color;

	RBTreeNode(const T& data)
		:_data(data)
		,_left(nullptr)
		,_right(nullptr)
		,_parent(nullptr)
		,_color(red)
	{}
};


// Iterator framework -> link -> operator
template<class T, class Ref, class Ptr>
struct RBTreeIterator {
	using Node = RBTreeNode<T>;
	using Self = RBTreeIterator<T, Ref, Ptr>;

	RBTreeIterator(Node* node)
		:_node(node)
	{}

	Self& operator++() {

		Node* parent = _node->_parent;

		if (_node->_right == nullptr){
			Node* cur = _node;

			while (parent && cur == parent->_right)
			{
				cur = parent;
				parent = parent->_parent;
			}
			_node = parent;
		}
		else if (_node->_right != nullptr) {
			Node* cur = _node->_right;
			while (cur->_left) {
				cur = cur->_left;
			}
			_node = cur;
			//parent = parent->_parent;
		}
		return *this;
	}

	Ref operator*()
	{
		return _node->_data;
	}

	Ptr operator->()
	{
		return &_node->_data;
	}

	bool operator!=(const Self& s) const
	{
		return _node != s._node;
	}

	bool operator==(const Self& s) const
	{
		return _node == s._node;
	}

public:
	Node* _node;
};

template<class K,class V,class KeyOfT>
class RBTree{
public:
	using Node = RBTreeNode<V>;
	using Iterator = RBTreeIterator<V, V&, V*>;

	RBTree()
		:_root(nullptr)
	{}

	Iterator begin()
	{
		Node* cur = _root;
		while (cur && cur->_left)
		{
			cur = cur->_left;
		}

		return Iterator(cur);
	}

	Iterator end()
	{
		return Iterator(nullptr);
	}

	void Insert(const V& kv) {
		if (_root == nullptr) {
			_root = new Node(kv);
			_root->_color = black;
			return;
		}

		KeyOfT t;
		Node* cur = _root;
		Node* parent = nullptr;
		while (cur)
		{
			if (t(kv)< t(cur->_data))
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (t(kv) > t(cur->_data))
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return ; // duplicate key
			}
		}

		Node* newnode = new Node(kv);

		if (t(newnode->_data) < t(parent->_data))
		{
			parent->_left = newnode;
		}
		else
		{
			parent->_right = newnode;
		}
		newnode->_parent = parent;

		while (newnode->_parent && newnode->_parent->_color == red)
		{
			Node* father = nullptr;
			Node* grandfa = nullptr;
			Node* uncle = nullptr;

			if (newnode->_parent) {
				father = newnode->_parent;
			}
			if (father) {
				grandfa = father->_parent;
			}
			if (grandfa) {
				if (grandfa->_left == father)
					uncle = grandfa->_right;
				else if (grandfa->_right == father)
					uncle = grandfa->_left;
				else {
					assert(false);
				}
			}

			if (father && father->_color == red) {
				if (uncle && uncle->_color == red) {
					father->_color = black;
					uncle->_color = black;
					if (grandfa == _root) {
						return;
					}
					else {
						grandfa->_color = red;
						newnode = grandfa;
					}
				}
				else if (uncle == nullptr || uncle->_color == black) {
					if (father == grandfa->_right && newnode == father->_right)
					{
						RotateL(grandfa);
						father->_color = black;
						grandfa->_color = red;
					}
					else if (father == grandfa->_right && newnode == father->_left)
					{
						RotateR(father);
						RotateL(grandfa);
						newnode->_color = black;
						grandfa->_color = red;
					}
					else if (father == grandfa->_left && newnode == father->_left) {
						grandfa->_color = red;
						father->_color = black;
						RotateR(grandfa);
					}
					else if (father == grandfa->_left && newnode == father->_right) {
						RotateL(father);
						RotateR(grandfa);
						newnode->_color = black;
						grandfa->_color = red;
					}
					else {
						assert(false);
					}
					break;
				}
			}
			else {
				break;
			}

		}
		_root->_color = black;
	}

	void RotateR(Node* parent) {
		Node* pParent = parent->_parent;
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		if (subLR)
		{
			subLR->_parent = parent;
		}
		if (parent == _root) {
			subL->_parent = nullptr;
			subL->_right = parent;
			_root = subL;
		}
		else {
			subL->_parent = pParent;
			subL->_right = parent;
			if (parent == pParent->_left) {
				pParent->_left = subL;
			}
			else {
				pParent->_right = subL;
			}
		}

		parent->_parent = subL;
		parent->_left = subLR;

	}

	void RotateL(Node* parent) {
		Node* pParent = parent->_parent;
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		if (subRL)
			subRL->_parent = parent;

		if (parent == _root) {
			subR->_parent = nullptr;
			subR->_left = parent;
			_root = subR;
		}
		else {
			subR->_parent = pParent;
			if (parent == pParent->_left)
				pParent->_left = subR;
			else
				pParent->_right = subR;
			subR->_left = parent;
		}

		parent->_parent = subR;
		parent->_right = subRL;

	}

	bool IsBalance() {
		return _IsBalance(_root);
	}

	// ==================== Public test/diagnostic interface ====================

	V* Find(const K& key) {
		KeyOfT t;
		Node* cur = _root;
		while (cur) {
			if (key < t(cur->_data))
				cur = cur->_left;
			else if (key > t(cur->_data))
				cur = cur->_right;
			else
				return &cur->_data;
		}
		return nullptr;
	}

	/// Total number of nodes
	int Size() {
		return _Size(_root);
	}

	/// In-order traversal print (for manual inspection of ordering)
	void PrintInOrder() {
		std::cout << "InOrder: ";
		_InOrderPrint(_root);
		std::cout << std::endl;
	}

	/// Validate all Red-Black tree properties (for automated testing)
	/// Returns true if the tree satisfies all RB invariants
	bool IsValidRBTree() {
		if (_root == nullptr) return true;
		if (_root->_color != black) {
			std::cout << "  [FAIL] Root is not black" << std::endl;
			return false;
		}
		return _Validate(_root) != -1;
	}

private:
	Node* _root;

	int _Size(Node* node) {
		if (!node) return 0;
		return 1 + _Size(node->_left) + _Size(node->_right);
	}

	void _InOrderPrint(Node* node) {
		if (!node) return;
		_InOrderPrint(node->_left);
		KeyOfT t;
		std::cout << "[" << t(node->_data)
			<< "(" << (node->_color == red ? "R" : "B") << ")] ";
		_InOrderPrint(node->_right);
	}

	int _Validate(Node* node) {
		if (!node) return 1;
		KeyOfT t;

		if (node->_color == red) {
			if ((node->_left  && node->_left->_color  == red) ||
				(node->_right && node->_right->_color == red)) {
				std::cout << "  [FAIL] Consecutive red nodes at: " << t(node->_data) << std::endl;
				return -1;
			}
		}

		if ((node->_left  && !(t(node->_left->_data)  < t(node->_data))) ||
			(node->_right && !(t(node->_data) < t(node->_right->_data)))) {
			std::cout << "  [FAIL] BST order violation at: " << t(node->_data) << std::endl;
			return -1;
		}

		int leftBH  = _Validate(node->_left);
		int rightBH = _Validate(node->_right);
		if (leftBH == -1 || rightBH == -1) return -1;
		if (leftBH != rightBH) {
			std::cout << "  [FAIL] Black-height mismatch at " << t(node->_data)
				<< " L=" << leftBH << " R=" << rightBH << std::endl;
			return -1;
		}

		return leftBH + (node->_color == black ? 1 : 0);
	}

	bool _IsBalance(Node* root) {
		if (root == nullptr) {
			return true;
		}

		int benchmark = 0;
		

		Node* cur = root;
		while (cur) {
			if (cur->_color == black) {
				++benchmark;
			}
			cur = cur->_left;
		}

		return _Check(root, 0, benchmark);
	}

	bool _Check(Node* root, int benchnum, int benchmark) {

		if (root == nullptr)
			return benchmark == benchnum;

		if (root->_color == black)	
			++benchnum;

		if (root->_color == red)
		{
			if (root->_left && root->_left->_color == red)
				return false;
			if (root->_right && root->_right->_color == red)
				return false;
		}

		return _Check(root->_left, benchnum, benchmark)
			&& _Check(root->_right, benchnum, benchmark);
	}
};
