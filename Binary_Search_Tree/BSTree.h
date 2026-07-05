#pragma once
#include<iostream>

namespace BSTree {
	template<class K>
	struct TreeNode
	{
	public:
		using Self = TreeNode;
		TreeNode(K value)
			:_value(value)
			, _left(nullptr)
			, _right(nullptr)
		{
		}

		K _value;
		Self* _left;
		Self* _right;
	};

	template<class K>
	class BSTree {
	public:
		//using Self = BSTree;


		BSTree(K value = 0)
			:_root(new TreeNode<K>(value))
			, _num(1)
		{}

		~BSTree() {
			Destory(_root);
			_root == nullptr;
		}

		//~BSTree() { 
		//	this->Erase();
		//	delete _root; 
		//}

		TreeNode<K>* get_root() {
			return _root;
		}

		void Insert(K value) {
			if (this->_root == nullptr) {
				_root = new TreeNode<K>(value);
				return;
			}
			TreeNode<K>* cur = _root;
			TreeNode<K>* parent = nullptr;

			while (cur != nullptr) {
				if (value > cur->_value) {
					parent = cur;
					cur = cur->_right;
				}
				else if (value <= cur->_value) {
					parent = cur;
					cur = cur->_left;
				}
			}
			if (value > parent->_value) {
				parent->_right = new TreeNode<K>(value);
			}
			else {
				parent->_left = new TreeNode<K>(value);
			}

			++_num;
		}

		void InOder() {
			_InOrder(get_root());
		}

		//不返回iterator
		void Search(K value) {
			TreeNode<K>* cur = _root;
			while (cur != nullptr) {
				if (cur->_value == value) {
					std::cout << "找到" << value << "了" << std::endl;
					return;
				}
				else if (value >= cur->_value) {
					cur = cur->_right;
				}
				else {
					cur = cur->_left;
				}
			}
			std::cout << "没找到" << std::endl;
		}

		void Erase(K value) {
			TreeNode<K>* cur = _root;
			TreeNode<K>* parent = cur;

			while (cur != nullptr) {
				if (cur->_value == value) {
					std::cout << "找到" << value << "了" << std::endl;
					//处理叶节点 及 左空或右空 逻辑
					if (cur->_left == nullptr) {			//左子树为空
						if (parent->_left == cur) {
							parent->_left = cur->_right;
						}
						else if (parent->_right == cur) {
							parent->_right = cur->_right;	//出现错误，将_left替代为_right
						}
						return;
					}
					else if (cur->_right == nullptr) {		//右子树为空
						if (parent->_left == cur) {
							parent->_left = cur->_left;		//出现错误，将_right替代为left
						}
						else if (parent->_right == cur) {
							parent->_right = cur->_left;
						}
						return;
					}
					else {									//处理左右均不为空逻辑
						TreeNode<K>* replace_parent = cur;
						TreeNode<K>* replace = cur->_right;

						while (replace->_left) {			//找到右子树最左节点
							replace_parent = replace;
							replace = replace->_left;
						}

						cur->_value = replace->_value;
						if (replace_parent->_left == replace)
						{
							replace_parent->_left = nullptr;
							return;
						}
						if (replace_parent->_right == replace)
						{
							replace_parent->_right = nullptr;
							return;
						}
					}
				}
				else if (value >= cur->_value) {
					parent = cur;
					cur = cur->_right;
				}
				else {
					parent = cur;
					cur = cur->_left;
				}
			}

		}

	private:

		void Destory(TreeNode<K>& root) {
			if (root == nullptr)
				return;
			Destory(root->_left);
			Destory(root->_right);
			delete root;
		}

		void _InOrder(TreeNode<K>* root = nullptr) {
			if (root == nullptr)
				return;
			_InOrder(root->_left);
			std::cout << root->_value << " ";
			_InOrder(root->_right);
		}

		TreeNode<K>* _root;
		int _num;
	};
}


namespace KVT {
	template<class K,class V>
	struct TreeNode
	{
	public:
		using Self = TreeNode;
		K _key;
		V _value;
		Self* _left;
		Self* _right;
	};

	template<class K,class V>
	class KVTree {
	public:
		void Insert(const K key,const V& vlaue) {
			if (this->_root == nullptr) {
				_root = new TreeNode<K,V>(key,value);
				return;
			}
			TreeNode<K,V>* cur = _root;
			TreeNode<K,V>* parent = nullptr;

			while (cur != nullptr) {
				if (value > cur->_value) {
					parent = cur;
					cur = cur->_right;
				}
				else if (value <= cur->_value) {
					parent = cur;
					cur = cur->_left;
				}
			}
			if (value > parent->_value) {
				parent->_right = new TreeNode<K,V>(value);
			}
			else {
				parent->_left = new TreeNode<K,V>(value);
			}

			++_num;
		}

	private:
			
	};
}