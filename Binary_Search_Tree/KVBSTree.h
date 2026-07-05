
#if 1
#pragma once
#include<iostream>
#include<string>

/**
 * =====================================================================
 *  KVBSTree.h — 基于二叉搜索树 (BST) 的 Key-Value 映射实现
 * =====================================================================
 *
 *  概述:
 *    本文件实现了一棵泛型的 Key-Value 二叉搜索树。
 *    树中每个节点存储一个键 (Key) 和一个值 (Value)。
 *    键用于比较和定位节点，值为该键对应的关联数据。
 *
 *  核心特性:
 *    - 按 Key 排序：左子树所有节点的 Key < 当前节点的 Key < 右子树所有节点的 Key
 *    - 不允许重复 Key（插入已存在的 Key 会更新对应的 Value）
 *    - 支持插入 (Insert)、查找 (Search)、删除 (Erase)、中序遍历 (InOrder)
 *    - 支持递归版本的插入、查找、删除
 *    - 使用 C++ 模板，可适用于任意可比较的 Key 类型和任意 Value 类型
 *
 *  时间复杂度 (平均情况):
 *    - 插入: O(log n)
 *    - 查找: O(log n)
 *    - 删除: O(log n)
 *    - 中序遍历: O(n)
 *    注意: 在最坏情况下（树退化为链表），上述操作退化为 O(n)
 *
 *  模板参数:
 *    K — 键的类型，必须支持 <, >, == 等比较运算符
 *    V — 值的类型，可以是任意类型
 * =====================================================================
 */

// =====================================================================
//  KVTreeNode — Key-Value 二叉搜索树的节点定义
// =====================================================================
/**
 * @brief Key-Value 树节点结构体
 *
 * @tparam K  键的类型（用于排序和查找）
 * @tparam V  值的类型（存储与键关联的数据）
 *
 * 每个节点包含:
 *   - _key   : 节点的键，用于在树中定位
 *   - _value : 节点的值，与键关联的实际数据
 *   - _left  : 指向左子节点的指针（左子树中所有 Key 都小于当前 Key）
 *   - _right : 指向右子节点的指针（右子树中所有 Key 都大于当前 Key）
 */
template<class K, class V>
struct KVTreeNode
{
public:
	// 类型别名，方便在内部引用自身类型
	using Self = KVTreeNode;

	/**
	 * @brief 构造函数 — 初始化一个 Key-Value 节点
	 * @param key   节点的键
	 * @param value 节点的值
	 *
	 * 初始化列表:
	 *   - _key   <- key   : 存储键
	 *   - _value <- value : 存储值
	 *   - _left  <- nullptr : 初始时没有左子节点
	 *   - _right <- nullptr : 初始时没有右子节点
	 */
	KVTreeNode(const K& key, const V& value)
		:_key(key)
		, _value(value)
		, _left(nullptr)
		, _right(nullptr)
	{}

	K _key;       // 键：用于比较、排序和定位
	V _value;     // 值：与键关联的实际数据
	Self* _left;  // 左子节点指针（所有 Key < 当前 Key 的节点）
	Self* _right; // 右子节点指针（所有 Key > 当前 Key 的节点）
};


// =====================================================================
//  KVBSTree — Key-Value 二叉搜索树类
// =====================================================================
/**
 * @brief Key-Value 二叉搜索树
 *
 * @tparam K  键的类型，必须支持比较运算符 (<, >, ==)
 * @tparam V  值的类型
 *
 * 该类封装了一棵以 Key 排序的二叉搜索树，提供:
 *   - 非递归的 插入 / 查找 / 删除
 *   - 递归的   插入 / 查找 / 删除
 *   - 中序遍历（按 Key 升序输出所有键值对）
 *   - 析构函数（递归释放所有节点内存，防止内存泄漏）
 *   - 拷贝构造和赋值运算符（深拷贝整棵树）
 */
template<class K, class V>
class KVBSTree {
public:
	// 节点类型别名，简化代码中对节点类型的引用
	using Node = KVTreeNode<K, V>;

	// =================================================================
	//  构造函数 / 析构函数 / 拷贝控制
	// =================================================================

	/**
	 * @brief 默认构造函数
	 *
	 * 创建一棵空的 Key-Value 二叉搜索树。
	 * 根节点指针初始化为 nullptr，节点计数初始化为 0。
	 */
	KVBSTree()
		:_root(nullptr)
		, _num(0)
	{}

	/**
	 * @brief 析构函数
	 *
	 * 递归销毁整棵树的所有节点，释放动态分配的内存。
	 * 调用私有辅助函数 _Destroy() 完成实际的递归销毁工作。
	 */
	~KVBSTree() {
		_Destroy(_root);
		_root = nullptr;
		_num = 0;
	}

	/**
	 * @brief 拷贝构造函数 — 深拷贝另一棵 KVBSTree
	 * @param other 要拷贝的源树
	 *
	 * 通过递归复制每个节点来创建一棵结构完全相同的新树。
	 * 保证新树和源树相互独立，修改一棵不会影响另一棵。
	 */
	KVBSTree(const KVBSTree& other)
		:_root(nullptr)
		, _num(0)
	{
		_root = _Copy(other._root);
		_num = other._num;
	}

	/**
	 * @brief 赋值运算符 — 使用 copy-and-swap 惯用法
	 * @param other 要赋值的源树（按值传递，触发拷贝构造）
	 * @return 返回当前对象的引用
	 *
	 * 实现原理:
	 *   1. 参数 other 是按值传递的，编译器会调用拷贝构造函数创建一个副本
	 *   2. 将当前对象的 _root 和 _num 与副本交换
	 *   3. 函数结束时，副本（持有原来的旧数据）被自动析构
	 *
	 * 优点: 异常安全，代码简洁，自动处理自赋值
	 */
	KVBSTree& operator=(KVBSTree other) {
		std::swap(_root, other._root);
		std::swap(_num, other._num);
		return *this;
	}

	// =================================================================
	//  公有接口 — 非递归版本
	// =================================================================

	/**
	 * @brief 获取根节点指针
	 * @return 指向根节点的指针，如果树为空则返回 nullptr
	 *
	 * 注意: 外部通过该指针可以直接访问树的内部结构，
	 *       仅建议在调试或遍历时使用。
	 */
	Node* GetRoot() {
		return _root;
	}

	/**
	 * @brief 获取树中节点数量
	 * @return 当前树中的节点总数
	 */
	size_t Size() const {
		return _num;
	}

	/**
	 * @brief 判断树是否为空
	 * @return 如果树中没有任何节点返回 true，否则返回 false
	 */
	bool Empty() const {
		return _root == nullptr;
	}

	/**
	 * @brief 插入一个键值对（非递归版本）
	 * @param key   要插入的键
	 * @param value 要插入的值
	 * @return 插入成功返回 true；如果 Key 已存在则更新 Value 并返回 false
	 *
	 * 算法流程:
	 *   1. 如果树为空，直接创建根节点
	 *   2. 否则，从根节点开始，根据 Key 的大小比较向左或向右遍历
	 *      - key > 当前节点的 key → 向右走
	 *      - key < 当前节点的 key → 向左走
	 *      - key == 当前节点的 key → 更新 value，返回 false
	 *   3. 找到空位置后，创建新节点并链接到父节点的对应位置
	 *
	 * 时间复杂度: O(h)，其中 h 为树的高度
	 */
	bool Insert(const K& key, const V& value) {
		// 情况 1: 树为空，直接创建根节点
		if (_root == nullptr) {
			_root = new Node(key, value);
			++_num;
			return true;
		}

		// 情况 2: 树非空，需要找到合适的插入位置
		Node* cur = _root;       // 当前遍历指针
		Node* parent = nullptr;  // 记录父节点，用于最后完成链接

		while (cur != nullptr) {
			if (key > cur->_key) {
				// 待插入的 key 大于当前节点 key，向右子树搜索
				parent = cur;
				cur = cur->_right;
			}
			else if (key < cur->_key) {
				// 待插入的 key 小于当前节点 key，向左子树搜索
				parent = cur;
				cur = cur->_left;
			}
			else {
				// key 相等：BST 不允许重复 key，这里选择更新 value
				cur->_value = value;
				return false;  // 返回 false 表示并非新插入，而是更新
			}
		}

		// 此时 cur == nullptr，parent 指向应插入位置的父节点
		// 根据 key 与 parent 的 key 比较结果决定插入左边还是右边
		if (key > parent->_key) {
			parent->_right = new Node(key, value);
		}
		else {
			parent->_left = new Node(key, value);
		}

		++_num;
		return true;
	}

	/**
	 * @brief 查找指定 Key 对应的节点（非递归版本）
	 * @param key 要查找的键
	 * @return 如果找到，返回指向该节点的指针；否则返回 nullptr
	 *
	 * 算法流程:
	 *   从根节点开始，逐层比较:
	 *   - key > 当前 key → 向右子树搜索
	 *   - key < 当前 key → 向左子树搜索
	 *   - key == 当前 key → 找到目标，返回节点指针
	 *   如果遍历到 nullptr，说明 key 不存在
	 *
	 * 时间复杂度: O(h)，其中 h 为树的高度
	 */
	Node* Search(const K& key) {
		Node* cur = _root;

		while (cur != nullptr) {
			if (key > cur->_key) {
				// 目标 key 较大，在右子树中继续查找
				cur = cur->_right;
			}
			else if (key < cur->_key) {
				// 目标 key 较小，在左子树中继续查找
				cur = cur->_left;
			}
			else {
				// 找到匹配的 key，返回该节点
				return cur;
			}
		}

		// 遍历结束仍未找到，返回空指针
		return nullptr;
	}

	/**
	 * @brief 删除指定 Key 的节点（非递归版本）
	 * @param key 要删除的键
	 * @return 删除成功返回 true；如果 Key 不存在返回 false
	 *
	 * 删除节点是 BST 中最复杂的操作，需考虑三种情况:
	 *
	 *   情况 1: 目标节点没有子节点（叶子节点）
	 *           → 直接删除该节点
	 *
	 *   情况 2: 目标节点只有一个子节点（只有左子树或只有右子树）
	 *           → 用其唯一的子节点替代该节点的位置
	 *
	 *   情况 3: 目标节点有两个子节点（左右子树都存在）
	 *           → 找到右子树中最小的节点（中序后继），
	 *             用该节点的 key/value 替换目标节点的 key/value，
	 *             然后删除那个中序后继节点（它最多只有一个右子节点）
	 *
	 * 时间复杂度: O(h)，其中 h 为树的高度
	 */
	bool Erase(const K& key) {
		Node* cur = _root;       // 当前遍历指针
		Node* parent = nullptr;  // 父节点指针

		// 第一步: 找到要删除的节点
		while (cur != nullptr) {
			if (key > cur->_key) {
				parent = cur;
				cur = cur->_right;
			}
			else if (key < cur->_key) {
				parent = cur;
				cur = cur->_left;
			}
			else {
				// 找到了目标节点 cur，开始执行删除逻辑
				break;
			}
		}

		// 如果没找到目标 key，返回 false
		if (cur == nullptr) {
			return false;
		}

		// 第二步: 根据子节点情况分类处理删除
		if (cur->_left == nullptr) {
			// -------------------------------------------------------
			// 情况 A: 目标节点没有左子树（可能有右子树，也可能无）
			// 直接用右子树替代当前节点
			// -------------------------------------------------------
			if (cur == _root) {
				// 特殊情况: 删除的是根节点
				_root = cur->_right;
			}
			else if (parent->_left == cur) {
				// 目标节点是父节点的左孩子
				parent->_left = cur->_right;
			}
			else {
				// 目标节点是父节点的右孩子
				parent->_right = cur->_right;
			}
			delete cur;
		}
		else if (cur->_right == nullptr) {
			// -------------------------------------------------------
			// 情况 B: 目标节点没有右子树（但有左子树）
			// 直接用左子树替代当前节点
			// -------------------------------------------------------
			if (cur == _root) {
				_root = cur->_left;
			}
			else if (parent->_left == cur) {
				parent->_left = cur->_left;
			}
			else {
				parent->_right = cur->_left;
			}
			delete cur;
		}
		else {
			// -------------------------------------------------------
			// 情况 C: 目标节点有两个子节点
			// 策略: 找右子树中最小的节点（中序后继）来替换
			//
			// 中序后继的性质:
			//   - 它是右子树中 Key 最小的节点
			//   - 它必定没有左子节点（否则它就不是最小的）
			//   - 用它替换目标节点后，BST 的有序性仍然保持
			// -------------------------------------------------------
			Node* successorParent = cur;     // 中序后继的父节点
			Node* successor = cur->_right;   // 从右子树开始找

			// 一直向左走，直到找到右子树中最小的节点
			while (successor->_left != nullptr) {
				successorParent = successor;
				successor = successor->_left;
			}

			// 用中序后继的 key 和 value 覆盖目标节点
			cur->_key = successor->_key;
			cur->_value = successor->_value;

			// 删除中序后继节点:
			// 中序后继没有左子节点，所以只需处理它的右子节点
			if (successorParent->_left == successor) {
				// 后继是其父节点的左孩子
				successorParent->_left = successor->_right;
			}
			else {
				// 后继是其父节点的右孩子（当 cur->_right 没有左子树时发生）
				successorParent->_right = successor->_right;
			}
			delete successor;
		}

		--_num;
		return true;
	}

	/**
	 * @brief 中序遍历 — 按 Key 升序输出所有键值对（公有接口）
	 *
	 * 中序遍历的顺序: 左子树 → 当前节点 → 右子树
	 * 对于 BST，中序遍历的结果恰好是按 Key 从小到大排列的。
	 *
	 * 输出格式: 每个键值对以 "[key:value] " 的形式打印
	 */
	void InOrder() {
		_InOrder(_root);
		std::cout << std::endl;
	}

	// =================================================================
	//  公有接口 — 递归版本
	// =================================================================

	/**
	 * @brief 插入键值对（递归版本的公有包装函数）
	 * @param key   要插入的键
	 * @param value 要插入的值
	 * @return 插入成功返回 true；Key 已存在则更新 Value 并返回 false
	 *
	 * 该函数将调用委托给私有递归函数 _InsertR()。
	 * 使用引用传递根指针，使得递归函数可以直接修改父节点的指针。
	 */
	bool InsertR(const K& key, const V& value) {
		return _InsertR(_root, key, value);
	}

	/**
	 * @brief 查找指定 Key 的节点（递归版本的公有包装函数）
	 * @param key 要查找的键
	 * @return 找到则返回节点指针，否则返回 nullptr
	 */
	Node* SearchR(const K& key) {
		return _SearchR(_root, key);
	}

	/**
	 * @brief 删除指定 Key 的节点（递归版本的公有包装函数）
	 * @param key 要删除的键
	 * @return 删除成功返回 true，Key 不存在返回 false
	 */
	bool EraseR(const K& key) {
		return _EraseR(_root, key);
	}

	/**
	 * @brief 通过下标运算符访问 / 插入键值对
	 * @param key 要访问的键
	 * @return 返回该键对应值的引用
	 *
	 * 行为说明:
	 *   - 如果 key 已存在，返回对应 value 的引用（可读可写）
	 *   - 如果 key 不存在，先插入 key 并使用 V 的默认值，再返回引用
	 *
	 * 这使得 KVBSTree 可以像 std::map 一样使用:
	 *   tree["hello"] = "world";
	 *   std::cout << tree["hello"];
	 */
	V& operator[](const K& key) {
		// 先尝试插入（如果已存在不会重复插入）
		Insert(key, V());
		// 然后查找该 key 对应的节点并返回其 value 的引用
		Node* node = Search(key);
		return node->_value;
	}

private:
	// =================================================================
	//  私有辅助函数 — 递归实现
	// =================================================================

	/**
	 * @brief 递归销毁以 root 为根的子树的所有节点
	 * @param root 子树的根节点指针
	 *
	 * 采用后序遍历的方式销毁:
	 *   1. 先递归销毁左子树
	 *   2. 再递归销毁右子树
	 *   3. 最后删除当前节点
	 *
	 * 采用后序遍历是因为必须先释放子节点，才能安全释放父节点。
	 * 如果先释放父节点，就会丢失对子节点的引用，造成内存泄漏。
	 */
	void _Destroy(Node* root) {
		if (root == nullptr) {
			return;
		}
		_Destroy(root->_left);   // 递归销毁左子树
		_Destroy(root->_right);  // 递归销毁右子树
		delete root;             // 释放当前节点
	}

	/**
	 * @brief 递归深拷贝以 root 为根的子树
	 * @param root 源子树的根节点指针
	 * @return 返回拷贝后新子树的根节点指针
	 *
	 * 采用前序遍历的方式拷贝:
	 *   1. 先复制当前节点
	 *   2. 递归复制左子树并链接
	 *   3. 递归复制右子树并链接
	 *
	 * 这样可以保证新树与源树的结构完全一致。
	 */
	Node* _Copy(Node* root) {
		if (root == nullptr) {
			return nullptr;
		}
		Node* newNode = new Node(root->_key, root->_value);  // 复制当前节点
		newNode->_left = _Copy(root->_left);                  // 递归复制左子树
		newNode->_right = _Copy(root->_right);                // 递归复制右子树
		return newNode;
	}

	/**
	 * @brief 递归中序遍历并输出键值对
	 * @param root 当前子树的根节点
	 *
	 * 中序遍历顺序: 左 → 根 → 右
	 * 对于 BST，这会按 Key 升序访问所有节点。
	 *
	 * 输出格式: [key:value]
	 */
	void _InOrder(Node* root) {
		if (root == nullptr) {
			return;
		}
		_InOrder(root->_left);                                          // 遍历左子树
		std::cout << "[" << root->_key << ":" << root->_value << "] ";  // 输出当前节点
		_InOrder(root->_right);                                         // 遍历右子树
	}

	/**
	 * @brief 递归插入键值对
	 * @param root  当前子树根节点的引用（引用传递，可直接修改父节点的指针）
	 * @param key   要插入的键
	 * @param value 要插入的值
	 * @return 插入成功返回 true，Key 已存在更新 value 后返回 false
	 *
	 * 递归逻辑:
	 *   - 基准情况: root == nullptr → 到达空位，创建新节点
	 *   - key > root->_key → 递归到右子树
	 *   - key < root->_key → 递归到左子树
	 *   - key == root->_key → 更新 value，返回 false
	 *
	 * 关键技巧: root 参数使用 Node*& （指针的引用），
	 *           这样当 root 为 nullptr 时，直接对 root 赋值新节点，
	 *           就等价于修改了父节点的 _left 或 _right 指针。
	 *           这消除了对「父节点」的单独追踪，代码更加简洁。
	 */
	bool _InsertR(Node*& root, const K& key, const V& value) {
		if (root == nullptr) {
			// 找到空位置，创建新节点
			root = new Node(key, value);
			++_num;
			return true;
		}

		if (key > root->_key) {
			return _InsertR(root->_right, key, value);  // 向右子树递归
		}
		else if (key < root->_key) {
			return _InsertR(root->_left, key, value);   // 向左子树递归
		}
		else {
			// Key 已存在，更新 value
			root->_value = value;
			return false;
		}
	}

	/**
	 * @brief 递归查找指定 Key 的节点
	 * @param root 当前子树的根节点
	 * @param key  要查找的键
	 * @return 找到返回节点指针，否则返回 nullptr
	 *
	 * 递归逻辑:
	 *   - root == nullptr → 未找到，返回 nullptr
	 *   - key > root->_key → 在右子树中查找
	 *   - key < root->_key → 在左子树中查找
	 *   - key == root->_key → 找到，返回当前节点
	 */
	Node* _SearchR(Node* root, const K& key) {
		if (root == nullptr) {
			return nullptr;
		}

		if (key > root->_key) {
			return _SearchR(root->_right, key);  // 在右子树中递归查找
		}
		else if (key < root->_key) {
			return _SearchR(root->_left, key);   // 在左子树中递归查找
		}
		else {
			return root;  // 找到目标节点
		}
	}

	/**
	 * @brief 递归删除指定 Key 的节点
	 * @param root 当前子树根节点的引用
	 * @param key  要删除的键
	 * @return 删除成功返回 true，Key 不存在返回 false
	 *
	 * 递归逻辑:
	 *   - root == nullptr → Key 不存在，返回 false
	 *   - key > root->_key → 在右子树中递归删除
	 *   - key < root->_key → 在左子树中递归删除
	 *   - key == root->_key → 找到目标节点，执行删除:
	 *       a) 无左子树 → 用右子树替代
	 *       b) 无右子树 → 用左子树替代
	 *       c) 双子树   → 用右子树最小节点（中序后继）替换，
	 *                      然后在右子树中递归删除该后继节点
	 *
	 * 同样使用指针引用技巧，简化了对父节点指针的修改。
	 */
	bool _EraseR(Node*& root, const K& key) {
		if (root == nullptr) {
			// 未找到目标 key
			return false;
		}

		if (key > root->_key) {
			return _EraseR(root->_right, key);  // 在右子树中递归删除
		}
		else if (key < root->_key) {
			return _EraseR(root->_left, key);   // 在左子树中递归删除
		}
		else {
			// 找到了要删除的节点
			Node* toDelete = root;  // 保存待删除节点的指针

			if (root->_left == nullptr) {
				// 情况 A: 没有左子树，用右子树替代
				root = root->_right;
				delete toDelete;
			}
			else if (root->_right == nullptr) {
				// 情况 B: 没有右子树，用左子树替代
				root = root->_left;
				delete toDelete;
			}
			else {
				// 情况 C: 左右子树都存在
				// 找右子树中最小的节点（中序后继）
				Node* successor = root->_right;
				while (successor->_left != nullptr) {
					successor = successor->_left;
				}

				// 用后继节点的 key/value 覆盖当前节点
				root->_key = successor->_key;
				root->_value = successor->_value;

				// 在右子树中递归删除后继节点
				// 因为后继节点的 key 已经确定，递归会精确找到并删除它
				return _EraseR(root->_right, successor->_key);
			}

			--_num;
			return true;
		}
	}

	// =================================================================
	//  成员变量
	// =================================================================
	Node* _root;   // 根节点指针，指向整棵树的根；空树时为 nullptr
	size_t _num;   // 树中节点的数量，用于快速获取大小
};
#endif