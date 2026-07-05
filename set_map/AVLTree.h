#pragma once
#include <iostream>
#include <utility>

template<class K, class V>
struct AVLTreeNode
{
    using Node = AVLTreeNode<K, V>;

    Node* _left;
    Node* _right;
    Node* _parent;
    int _bf;
    std::pair<K, V> _kv;

    AVLTreeNode(const std::pair<K, V>& kv)
        : _left(nullptr)
        , _right(nullptr)
        , _parent(nullptr)
        , _bf(0)
        , _kv(kv)
    {
    }
};

template<class K, class V>
class AVLTree
{
    using Node = AVLTreeNode<K, V>;

    template<class, class>
    friend struct AVLTreeAccessor;

public:
    AVLTree()
        : _root(nullptr)
    {
    }

    bool Insert(const std::pair<K, V>& kv)
    {
        if (_root == nullptr)
        {
            _root = new Node(kv);
            return true;
        }

        Node* cur = _root;
        Node* parent = nullptr;

        while (cur)
        {
            if (kv.first < cur->_kv.first)
            {
                parent = cur;
                cur = cur->_left;
            }
            else if (kv.first > cur->_kv.first)
            {
                parent = cur;
                cur = cur->_right;
            }
            else
            {
                return false; // duplicate key
            }
        }

        Node* newnode = new Node(kv);

        if (newnode->_kv.first < parent->_kv.first)
        {
            parent->_left = newnode;
        }
        else
        {
            parent->_right = newnode;
        }
        newnode->_parent = parent;

        Node* tmp = newnode;

        while (parent) {

            if (parent->_right == tmp) {
                parent->_bf++;
            }
            else if (parent->_left == tmp) {
                parent->_bf--;
            }

            if (parent->_bf == 2 || parent->_bf == -2) {
                if (parent->_bf == -2 && parent->_left && parent->_left->_bf == -1)
                    RotateR(parent);
                else if (parent->_bf == 2 && parent->_right && parent->_right->_bf == 1)
                    RotateL(parent);
                else if (parent->_bf == -2 && parent->_left && parent->_left->_bf == 1)
                    RotateLR(parent);
                else if (parent->_bf == 2 && parent->_right && parent->_right->_bf == -1)
                    RotateRL(parent);
                break;
            }
            else if (parent->_bf == 0) {
                break;
            }
            if (parent->_bf == 1 || parent->_bf == -1) {
                tmp = parent;
                parent = parent->_parent;
            }

        }
        return true;
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
            else{
                pParent->_right = subL;
            }
        }

        parent->_parent = subL;
        parent->_left = subLR;

        subL->_bf = 0;
        parent->_bf = 0;
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

        subR->_bf = 0;
        parent->_bf = 0;
    }

    void RotateLR(Node* parent) {
        Node* subL = parent->_left;
        Node* subLR = subL->_right;
        int bf = subLR->_bf;

        RotateL(parent->_left);
        RotateR(parent);

        if (bf == -1) {
            subL->_bf = 0;
            parent->_bf = 1;
            subLR->_bf = 0;
        }
        else if (bf == 1) {
            subL->_bf = -1;
            parent->_bf = 0;
            subLR->_bf = 0;
        }
        else {
            subL->_bf = 0;
            parent->_bf = 0;
            subLR->_bf = 0;
        }
    }

    void RotateRL(Node* parent) {
        Node* subR = parent->_right;
        Node* subRL = subR->_left;
        int bf = subRL->_bf;

        RotateR(parent->_right);
        RotateL(parent);

        if (bf == -1) {
            subR->_bf = 1;
            parent->_bf = 0;
            subRL->_bf = 0;
        }
        else if (bf == 1) {
            subR->_bf = 0;
            parent->_bf = -1;
            subRL->_bf = 0;
        }
        else {
            subR->_bf = 0;
            parent->_bf = 0;
            subRL->_bf = 0;
        }
    }

    void InOrder()
    {
        _InOrder(_root);
        std::cout << std::endl;
    }

private:
    void _InOrder(Node* root)
    {
        if (root == nullptr)
        {
            return;
        }

        _InOrder(root->_left);
        std::cout << root->_kv.first << ":" << root->_kv.second << " ";
        _InOrder(root->_right);
    }

private:
    Node* _root;
};
