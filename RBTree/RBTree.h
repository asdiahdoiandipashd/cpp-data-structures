#pragma once
#include<iostream>
#include<utility>
#include<assert.h>

enum NodeColor{
	red,
	black
};

template<class K, class V>
struct RBTreeNode
{
	using Node = RBTreeNode<K,V>;
	std::pair<K, V> _kv;
	Node* _left;
	Node* _right;
	Node* _parent;
	NodeColor _color;

	RBTreeNode(const std::pair<K,V>& kv)
		:_kv(kv)
		,_left(nullptr)
		,_right(nullptr)
		,_parent(nullptr)
		,_color(red)
	{}
};

template<class K,class V,class KeyOfT>
class RBTree{
public:
	using Node = RBTreeNode<K,V>;
	RBTree()
		:_root(nullptr)
	{}

	void Insert(const V& kv) {
		if (_root == nullptr) {
			_root = new Node(kv);
			_root->_color = black;
			return;
		}

		Node* cur = _root;
		Node* parent = nullptr;
		while (cur)
		{
			if (KeyOfT(kv) < KeyOfT(cur->_kv))
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (KeyOfT(kv) > KeyOfT(cur->_kv))
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

		if (KeyOfT(newnode->_kv) < KeyOfT(parent->_kv))
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

	/// Find a key, return pointer to value, or nullptr if not found
	V* Find(const K& key) {
		Node* cur = _root;
		while (cur) {
			if (key < cur->_kv.first)
				cur = cur->_left;
			else if (key > cur->_kv.first)
				cur = cur->_right;
			else
				return &cur->_kv.second;
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
		std::cout << "[" << node->_kv.first << ":" << node->_kv.second
			<< "(" << (node->_color == red ? "R" : "B") << ")] ";
		_InOrderPrint(node->_right);
	}

	/// Recursively validate RB properties. Returns black-height of subtree, or -1 on violation.
	int _Validate(Node* node) {
		if (!node) return 1;  // null leaf counts as black, black-height = 1

		// Red node must have black children (no consecutive red)
		if (node->_color == red) {
			if ((node->_left  && node->_left->_color  == red) ||
				(node->_right && node->_right->_color == red)) {
				std::cout << "  [FAIL] Consecutive red nodes at: " << node->_kv.first << std::endl;
				return -1;
			}
		}

		// BST property: left < current < right
		if ((node->_left  && !(node->_left->_kv.first  < node->_kv.first)) ||
			(node->_right && !(node->_kv.first < node->_right->_kv.first))) {
			std::cout << "  [FAIL] BST order violation at: " << node->_kv.first << std::endl;
			return -1;
		}

		int leftBH  = _Validate(node->_left);
		int rightBH = _Validate(node->_right);
		if (leftBH == -1 || rightBH == -1) return -1;
		if (leftBH != rightBH) {
			std::cout << "  [FAIL] Black-height mismatch at " << node->_kv.first
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
		
		//通过循环得到一条链路的黑色节点数量 benchnum
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
		//如果走到空指针，情况一：走完该条链路；情况二：为空树
		//之后进行benchnum和benchmark的比较
		if (root == nullptr)
			//若数量相等，则证明红黑树平衡
			return benchmark == benchnum;

		//收集该条链路黑色节点数量
		if (root->_color == black)	
			++benchnum;

		//红节点处理方法：左右子树有一个为红则为不平衡
		if (root->_color == red)
		{
			if (root->_left && root->_left->_color == red)
				return false;
			if (root->_right && root->_right->_color == red)
				return false;
		}

		//分别递归左右子树，
		return _Check(root->_left, benchnum, benchmark)
			&& _Check(root->_right, benchnum, benchmark);
	}
};
