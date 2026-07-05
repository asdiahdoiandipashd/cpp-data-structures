#if 1
/**
 * =====================================================================
 *  KVBSTree_test.cpp — Key-Value 二叉搜索树的功能测试
 * =====================================================================
 *
 *  本文件演示 KVBSTree 的各种操作，包括:
 *    1. 基本的插入和中序遍历
 *    2. 查找功能
 *    3. 删除功能
 *    4. operator[] 下标访问（类似 std::map 用法）
 *    5. 实际应用场景: 统计单词出现次数
 *    6. 拷贝构造和赋值运算符
 * =====================================================================
 */
#include "KVBSTree.h"
#include <string>

/**
 * @brief 测试 1: 基本的插入与中序遍历
 *
 * 演示:
 *   - 向树中插入多个 int→string 键值对
 *   - 中序遍历输出（结果按 key 升序排列）
 *   - 插入重复 key 时会更新 value
 */
void TestInsertAndTraversal() {
	std::cout << "===== 测试 1: 插入与中序遍历 =====" << std::endl;

	KVBSTree<int, std::string> tree;

	// 插入若干键值对
	tree.Insert(5, "五");
	tree.Insert(3, "三");
	tree.Insert(7, "七");
	tree.Insert(1, "一");
	tree.Insert(4, "四");
	tree.Insert(6, "六");
	tree.Insert(8, "八");

	std::cout << "插入 7 个节点后的中序遍历: " << std::endl;
	tree.InOrder();
	// 期望输出: [1:一] [3:三] [4:四] [5:五] [6:六] [7:七] [8:八]

	// 测试重复 key 插入（应更新 value）
	tree.Insert(5, "伍（更新后）");
	std::cout << "更新 key=5 的 value 后: " << std::endl;
	tree.InOrder();
	// 期望输出: [1:一] [3:三] [4:四] [5:伍（更新后）] [6:六] [7:七] [8:八]

	std::cout << "节点总数: " << tree.Size() << std::endl;
	std::cout << std::endl;
}

/**
 * @brief 测试 2: 查找功能
 *
 * 演示:
 *   - 查找存在的 key（返回节点指针）
 *   - 查找不存在的 key（返回 nullptr）
 *   - 通过返回的节点指针修改 value
 */
void TestSearch() {
	std::cout << "===== 测试 2: 查找功能 =====" << std::endl;

	KVBSTree<std::string, int> tree;
	tree.Insert("apple", 10);
	tree.Insert("banana", 20);
	tree.Insert("cherry", 30);
	tree.Insert("date", 40);

	// 查找存在的 key
	auto* node1 = tree.Search("banana");
	if (node1) {
		std::cout << "找到 'banana': value = " << node1->_value << std::endl;
	}

	// 查找不存在的 key
	auto* node2 = tree.Search("grape");
	if (node2 == nullptr) {
		std::cout << "未找到 'grape'" << std::endl;
	}

	// 通过指针修改 value
	if (node1) {
		node1->_value = 200;
		std::cout << "修改 'banana' 的 value 为 200 后: " << std::endl;
		tree.InOrder();
	}

	std::cout << std::endl;
}

/**
 * @brief 测试 3: 删除功能
 *
 * 演示删除的三种情况:
 *   - 删除叶子节点
 *   - 删除只有一个子节点的节点
 *   - 删除有两个子节点的节点
 *   - 删除根节点
 *   - 尝试删除不存在的 key
 */
void TestErase() {
	std::cout << "===== 测试 3: 删除功能 =====" << std::endl;

	KVBSTree<int, std::string> tree;
	tree.Insert(50, "根");
	tree.Insert(30, "左");
	tree.Insert(70, "右");
	tree.Insert(20, "左左");
	tree.Insert(40, "左右");
	tree.Insert(60, "右左");
	tree.Insert(80, "右右");

	std::cout << "初始状态: ";
	tree.InOrder();

	// 删除叶子节点
	tree.Erase(20);
	std::cout << "删除叶子节点 20 后: ";
	tree.InOrder();

	// 删除只有一个子节点的节点
	tree.Erase(30);
	std::cout << "删除单子节点 30 后: ";
	tree.InOrder();

	// 删除有两个子节点的节点
	tree.Erase(70);
	std::cout << "删除双子节点 70 后: ";
	tree.InOrder();

	// 删除根节点
	tree.Erase(50);
	std::cout << "删除根节点 50 后: ";
	tree.InOrder();

	// 尝试删除不存在的 key
	bool result = tree.Erase(999);
	std::cout << "删除不存在的 key 999: " << (result ? "成功" : "失败（不存在）") << std::endl;

	std::cout << "剩余节点数: " << tree.Size() << std::endl;
	std::cout << std::endl;
}

/**
 * @brief 测试 4: operator[] 下标运算符
 *
 * 演示类似 std::map 的用法:
 *   - 通过 [] 插入新键值对
 *   - 通过 [] 读取已有的值
 *   - 通过 [] 修改已有的值
 */
void TestOperatorBracket() {
	std::cout << "===== 测试 4: operator[] 下标运算符 =====" << std::endl;

	KVBSTree<std::string, int> tree;

	// 通过 [] 插入
	tree["math"] = 95;
	tree["english"] = 88;
	tree["physics"] = 92;
	tree["chemistry"] = 85;

	std::cout << "通过 [] 插入 4 门课程成绩: ";
	tree.InOrder();

	// 通过 [] 读取
	std::cout << "math 的成绩: " << tree["math"] << std::endl;

	// 通过 [] 修改
	tree["english"] = 99;
	std::cout << "修改 english 成绩为 99 后: ";
	tree.InOrder();

	std::cout << std::endl;
}

/**
 * @brief 测试 5: 实际应用 — 统计单词出现次数
 *
 * 这是 Key-Value BST 最经典的应用场景之一:
 *   - Key = 单词 (std::string)
 *   - Value = 出现次数 (int)
 *   - 使用 operator[] 简洁地实现计数逻辑
 */
void TestWordCount() {
	std::cout << "===== 测试 5: 单词计数 =====" << std::endl;

	// 模拟一段文本中的单词
	std::string words[] = {
		"hello", "world", "hello", "cpp", "bst",
		"hello", "world", "tree", "bst", "hello",
		"cpp", "data", "structure", "tree", "bst"
	};

	KVBSTree<std::string, int> wordCount;

	// 统计每个单词出现的次数
	for (const auto& word : words) {
		wordCount[word]++;  // 第一次出现时默认 value=0，然后 +1
	}

	std::cout << "单词出现次数统计 (按字母序): " << std::endl;
	wordCount.InOrder();

	// 查询特定单词的出现次数
	auto* node = wordCount.Search("hello");
	if (node) {
		std::cout << "'hello' 出现了 " << node->_value << " 次" << std::endl;
	}

	std::cout << "不同单词总数: " << wordCount.Size() << std::endl;
	std::cout << std::endl;
}

/**
 * @brief 测试 6: 拷贝构造和赋值运算符
 *
 * 验证深拷贝的正确性:
 *   - 拷贝构造后两棵树内容一致
 *   - 修改一棵树不影响另一棵
 */
void TestCopyAndAssign() {
	std::cout << "===== 测试 6: 拷贝构造与赋值 =====" << std::endl;

	KVBSTree<int, std::string> tree1;
	tree1.Insert(10, "十");
	tree1.Insert(5, "五");
	tree1.Insert(15, "十五");
	tree1.Insert(3, "三");
	tree1.Insert(7, "七");

	std::cout << "原始树 tree1: ";
	tree1.InOrder();

	// 测试拷贝构造
	KVBSTree<int, std::string> tree2(tree1);
	std::cout << "拷贝构造 tree2: ";
	tree2.InOrder();

	// 修改 tree2，验证不影响 tree1
	tree2.Insert(20, "二十");
	tree2.Erase(3);
	std::cout << "修改 tree2 后: ";
	tree2.InOrder();
	std::cout << "tree1 不受影响: ";
	tree1.InOrder();

	// 测试赋值运算符
	KVBSTree<int, std::string> tree3;
	tree3.Insert(100, "百");
	tree3 = tree1;
	std::cout << "tree3 = tree1 赋值后: ";
	tree3.InOrder();

	std::cout << std::endl;
}

/**
 * @brief 测试 7: 递归版本的插入、查找、删除
 *
 * 验证递归版本的功能与非递归版本一致
 */
void TestRecursiveOperations() {
	std::cout << "===== 测试 7: 递归版本 =====" << std::endl;

	KVBSTree<int, std::string> tree;

	// 使用递归版本插入
	tree.InsertR(50, "五十");
	tree.InsertR(25, "二十五");
	tree.InsertR(75, "七十五");
	tree.InsertR(10, "十");
	tree.InsertR(30, "三十");

	std::cout << "递归插入 5 个节点: ";
	tree.InOrder();

	// 使用递归版本查找
	auto* node = tree.SearchR(25);
	if (node) {
		std::cout << "递归查找 key=25: value=" << node->_value << std::endl;
	}

	auto* node2 = tree.SearchR(999);
	std::cout << "递归查找 key=999: " << (node2 ? "找到" : "未找到") << std::endl;

	// 使用递归版本删除
	tree.EraseR(25);
	std::cout << "递归删除 key=25 后: ";
	tree.InOrder();

	tree.EraseR(50);
	std::cout << "递归删除 key=50 后: ";
	tree.InOrder();

	std::cout << std::endl;
}

/**
 * @brief 主函数 — 依次运行所有测试
 */
int main() {
	TestInsertAndTraversal();   // 测试 1: 插入与遍历
	TestSearch();               // 测试 2: 查找
	TestErase();                // 测试 3: 删除
	TestOperatorBracket();      // 测试 4: [] 下标运算符
	TestWordCount();            // 测试 5: 单词计数应用
	TestCopyAndAssign();        // 测试 6: 拷贝与赋值
	TestRecursiveOperations();  // 测试 7: 递归版本

	return 0;
}
#endif