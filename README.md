# C++ Data Structures

C++ 手写树形数据结构合集，涵盖从基础二叉搜索树到红黑树封装 set/map 的完整实现。

## 项目结构

```
.
├── Binary_Search_Tree/   # 二叉搜索树（BST）
│   ├── BSTree.h          # 基础 BST（模板，按值搜索）
│   ├── BSTree.cpp        # BST 测试
│   ├── KVBSTree.h        # Key-Value BST（支持 operator[]、递归/非递归双版本）
│   ├── KVBSTree_test.cpp # KVBSTree 测试（词频统计等）
│   └── main.cpp
│
├── AVLTree/              # AVL 平衡二叉搜索树
│   ├── AVLTree.h         # AVL 树（LL/RR/LR/RL 四种旋转）
│   └── AVLTree.cpp       # AVL 测试
│
├── RBTree/               # 红黑树
│   ├── RBTree.h          # 红黑树（插入 + 旋转 + 变色 + 平衡验证）
│   ├── main.cpp          # 红黑树测试
│   └── test.cpp          # 自动化验证测试
│
├── map/                  # std::map 用法示例
│   └── main.cpp          # map 基本操作演示
│
└── set_map/              # 基于红黑树封装 set 和 map
    ├── RBTree.h          # 泛型红黑树（KeyOfT 仿函数 + 迭代器）
    ├── AVLTree.h         # AVL 树（同 AVLTree/ 目录）
    ├── set.h             # myset —— 基于 RBTree 封装
    ├── map.h             # mymap —— 基于 RBTree 封装
    └── main.cpp          # set/map 测试
```

## 各模块说明

### Binary_Search_Tree

基础二叉搜索树，包含两个版本：

- **BSTree** — 单值 BST，支持插入、中序遍历、查找、删除
- **KVBSTree** — Key-Value BST，功能更完善：
  - 非递归 + 递归双版本的 Insert / Search / Erase
  - `operator[]` 支持（类似 `std::map` 用法）
  - 深拷贝（拷贝构造 + copy-and-swap 赋值）
  - 删除三种情况完整处理（叶子节点 / 单子树 / 双子树）

### AVLTree

自平衡二叉搜索树，在 BST 基础上通过平衡因子（Balance Factor）和旋转保持树高平衡：

| 失衡类型 | 旋转方式 |
|---------|---------|
| LL | 右单旋 `RotateR` |
| RR | 左单旋 `RotateL` |
| LR | 先左旋后右旋 `RotateLR` |
| RL | 先右旋后左旋 `RotateRL` |

插入后沿路径向上更新平衡因子，遇到 |bf| == 2 时触发旋转。

### RBTree

红黑树实现，满足五大性质：

1. 节点非红即黑
2. 根节点为黑
3. 叶子（NIL）为黑
4. 红节点的子节点必须为黑
5. 任意节点到叶子的所有路径上黑节点数相同

包含完整的插入调整逻辑（叔叔红/叔叔黑四种旋转情况）和 `IsValidRBTree()` 自动验证方法。

### set_map

在红黑树基础上，通过 **KeyOfT 仿函数**模式实现一棵树同时支持 set 和 map：

- `RBTree<K, V, KeyOfT>` — 泛型红黑树，V 可以是 `Key`（set）或 `pair<Key, Value>`（map）
- `RBTreeIterator` — 中序遍历迭代器（`operator++` / `operator*` / `operator->`）
- `myset<Key>` — 封装 RBTree，KeyOfT 直接返回 Key
- `mymap<Key, Value>` — 封装 RBTree，KeyOfT 返回 `pair.first`

## 编译运行

使用 Visual Studio 2022 打开对应 `.sln` 文件直接编译，或使用命令行：

```bash
# 以 AVLTree 为例
cd AVLTree
g++ -std=c++17 -o avltree AVLTree.cpp
./avltree
```

## 复杂度总结

| 数据结构 | 查找 | 插入 | 删除 |
|---------|------|------|------|
| BST（平均） | O(log n) | O(log n) | O(log n) |
| BST（最坏） | O(n) | O(n) | O(n) |
| AVL 树 | O(log n) | O(log n) | O(log n) |
| 红黑树 | O(log n) | O(log n) | O(log n) |
