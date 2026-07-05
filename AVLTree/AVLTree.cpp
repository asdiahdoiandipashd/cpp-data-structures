#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "AVLTree.h"

static int g_passed = 0;
static int g_failed = 0;

#define TEST(name) \
    do { \
        std::cout << "[TEST] " << name << " ... "; \
        try {

#define END_TEST \
            std::cout << "PASSED" << std::endl; \
            ++g_passed; \
        } catch (const std::exception& e) { \
            std::cout << "FAILED (" << e.what() << ")" << std::endl; \
            ++g_failed; \
        } \
    } while(0);

// Helper: post-order check balance, return height; -1 = unbalanced
template<class K, class V>
static int CheckBalance(AVLTreeNode<K, V>* node) {
    if (node == nullptr) return 0;
    int leftH = CheckBalance(node->_left);
    int rightH = CheckBalance(node->_right);
    if (leftH == -1 || rightH == -1) return -1;
    int diff = rightH - leftH;
    if (diff < -1 || diff > 1) {
        std::cerr << std::endl
                  << "  Height imbalance: key=" << node->_kv.first
                  << " leftH=" << leftH << " rightH=" << rightH << std::endl;
        return -1;
    }
    if (node->_bf != diff) {
        std::cerr << std::endl
                  << "  BF mismatch: key=" << node->_kv.first
                  << " expected=" << diff << " actual=" << node->_bf << std::endl;
        return -1;
    }
    return std::max(leftH, rightH) + 1;
}

template<class K, class V>
static int CountNodes(AVLTreeNode<K, V>* node) {
    if (node == nullptr) return 0;
    return 1 + CountNodes(node->_left) + CountNodes(node->_right);
}

template<class K, class V>
static void CollectInOrder(AVLTreeNode<K, V>* node, std::vector<std::pair<K, V>>& out) {
    if (node == nullptr) return;
    CollectInOrder(node->_left, out);
    out.push_back(node->_kv);
    CollectInOrder(node->_right, out);
}

template<class K, class V>
struct AVLTreeAccessor {
    static AVLTreeNode<K, V>* GetRoot(AVLTree<K, V>& tree) {
        return tree._root;
    }
};

// Verify balance + node count via Accessor
template<class K, class V>
static void VerifyBalanceAndNodeCount(AVLTree<K, V>& tree, int expectedCount) {
    auto* root = AVLTreeAccessor<K, V>::GetRoot(tree);
    int h = CheckBalance(root);
    if (h == -1)
        throw std::runtime_error("Tree unbalanced or BF error");
    int n = CountNodes(root);
    if (n != expectedCount)
        throw std::runtime_error(
            "Node count mismatch: expected=" + std::to_string(expectedCount)
            + " actual=" + std::to_string(n));
}

template<class K, class V>
static void VerifyBalance(AVLTree<K, V>& tree) {
    auto* root = AVLTreeAccessor<K, V>::GetRoot(tree);
    if (CheckBalance(root) == -1)
        throw std::runtime_error("Tree unbalanced or BF error");
}

// =============================================================
// Test Cases
// =============================================================

// ---- Basic ----

void TestEmptyTree() {
    TEST("Empty tree")
    AVLTree<int, int> tree;
    VerifyBalanceAndNodeCount(tree, 0);
    END_TEST
}

void TestSingleInsert() {
    TEST("Single insert + balance check")
    AVLTree<int, int> tree;
    assert(tree.Insert({10, 100}));
    VerifyBalanceAndNodeCount(tree, 1);
    END_TEST
}

void TestDuplicateKey() {
    TEST("Duplicate key rejection")
    AVLTree<int, int> tree;
    assert(tree.Insert({5, 100}));
    assert(!tree.Insert({5, 200}));
    assert(!tree.Insert({5, 300}));
    VerifyBalanceAndNodeCount(tree, 1);
    END_TEST
}

void TestMultipleInsertsNoRotation() {
    TEST("Multiple inserts (no rotation) + balance")
    AVLTree<int, int> tree;
    assert(tree.Insert({50, 50}));
    assert(tree.Insert({30, 30}));
    assert(tree.Insert({70, 70}));
    assert(tree.Insert({20, 20}));
    assert(tree.Insert({40, 40}));
    assert(tree.Insert({60, 60}));
    assert(tree.Insert({80, 80}));
    VerifyBalanceAndNodeCount(tree, 7);
    std::cout << std::endl << "  InOrder: ";
    tree.InOrder();
    END_TEST
}

void TestNodeCount() {
    TEST("10 nodes + count verification")
    AVLTree<int, int> tree;
    for (int i = 1; i <= 10; ++i)
        tree.Insert({i, i});
    VerifyBalanceAndNodeCount(tree, 10);
    std::cout << std::endl << "  InOrder: ";
    tree.InOrder();
    END_TEST
}

// ---- Rotations ----

void TestLLRotation() {
    TEST("LL rotation (right rotate) -- 3,2,1")
    AVLTree<int, int> tree;
    assert(tree.Insert({3, 3}));
    assert(tree.Insert({2, 2}));
    assert(tree.Insert({1, 1}));
    VerifyBalanceAndNodeCount(tree, 3);
    std::cout << std::endl << "  InOrder: ";
    tree.InOrder();
    END_TEST
}

void TestRRRotation() {
    TEST("RR rotation (left rotate) -- 1,2,3")
    AVLTree<int, int> tree;
    assert(tree.Insert({1, 1}));
    assert(tree.Insert({2, 2}));
    assert(tree.Insert({3, 3}));
    VerifyBalanceAndNodeCount(tree, 3);
    std::cout << std::endl << "  InOrder: ";
    tree.InOrder();
    END_TEST
}

void TestLRRotation() {
    TEST("LR double rotation -- 3,1,2")
    AVLTree<int, int> tree;
    assert(tree.Insert({3, 3}));
    assert(tree.Insert({1, 1}));
    assert(tree.Insert({2, 2}));
    VerifyBalanceAndNodeCount(tree, 3);
    std::cout << std::endl << "  InOrder: ";
    tree.InOrder();
    END_TEST
}

void TestRLRotation() {
    TEST("RL double rotation -- 1,3,2")
    AVLTree<int, int> tree;
    assert(tree.Insert({1, 1}));
    assert(tree.Insert({3, 3}));
    assert(tree.Insert({2, 2}));
    VerifyBalanceAndNodeCount(tree, 3);
    std::cout << std::endl << "  InOrder: ";
    tree.InOrder();
    END_TEST
}

void TestLLRotationExtended() {
    TEST("LL rotation extended -- 4,3,2,1")
    AVLTree<int, int> tree;
    assert(tree.Insert({4, 4}));
    assert(tree.Insert({3, 3}));
    assert(tree.Insert({2, 2}));
    assert(tree.Insert({1, 1}));
    VerifyBalanceAndNodeCount(tree, 4);
    std::cout << std::endl << "  InOrder: ";
    tree.InOrder();
    END_TEST
}

void TestRRRotationExtended() {
    TEST("RR rotation extended -- 1,2,3,4")
    AVLTree<int, int> tree;
    assert(tree.Insert({1, 1}));
    assert(tree.Insert({2, 2}));
    assert(tree.Insert({3, 3}));
    assert(tree.Insert({4, 4}));
    VerifyBalanceAndNodeCount(tree, 4);
    std::cout << std::endl << "  InOrder: ";
    tree.InOrder();
    END_TEST
}

void TestMixedRotations() {
    TEST("Mixed rotations -- 9 nodes + balance")
    AVLTree<int, int> tree;
    assert(tree.Insert({5, 5}));
    assert(tree.Insert({3, 3}));
    assert(tree.Insert({7, 7}));
    assert(tree.Insert({2, 2}));
    assert(tree.Insert({4, 4}));
    assert(tree.Insert({6, 6}));
    assert(tree.Insert({8, 8}));
    assert(tree.Insert({1, 1}));
    assert(tree.Insert({9, 9}));
    VerifyBalanceAndNodeCount(tree, 9);
    std::cout << std::endl << "  InOrder: ";
    tree.InOrder();
    END_TEST
}

void TestLRRotationExtended() {
    TEST("LR double rotation extended -- 5,2,3")
    AVLTree<int, int> tree;
    assert(tree.Insert({5, 5}));
    assert(tree.Insert({2, 2}));
    assert(tree.Insert({3, 3}));
    VerifyBalanceAndNodeCount(tree, 3);
    std::cout << std::endl << "  InOrder: ";
    tree.InOrder();
    END_TEST
}

void TestRLRotationExtended() {
    TEST("RL double rotation extended -- 2,5,4")
    AVLTree<int, int> tree;
    assert(tree.Insert({2, 2}));
    assert(tree.Insert({5, 5}));
    assert(tree.Insert({4, 4}));
    VerifyBalanceAndNodeCount(tree, 3);
    std::cout << std::endl << "  InOrder: ";
    tree.InOrder();
    END_TEST
}

// ---- Large Scale ----

void TestLargeSequential100() {
    TEST("Sequential 100 nodes + balance")
    AVLTree<int, int> tree;
    for (int i = 1; i <= 100; ++i)
        assert(tree.Insert({i, i}));
    VerifyBalanceAndNodeCount(tree, 100);
    END_TEST
}

void TestLargeReverse100() {
    TEST("Reverse 100 nodes + balance")
    AVLTree<int, int> tree;
    for (int i = 100; i >= 1; --i)
        assert(tree.Insert({i, i}));
    VerifyBalanceAndNodeCount(tree, 100);
    END_TEST
}

void TestRandomInsert() {
    TEST("Random 50 nodes + balance")
    AVLTree<int, int> tree;
    std::srand(42);
    int count = 0;
    for (int i = 0; i < 50; ++i) {
        int v = std::rand() % 200;
        if (tree.Insert({v, v})) ++count;
    }
    VerifyBalanceAndNodeCount(tree, count);
    END_TEST
}

// ---- Key Ordering ----

void TestKeyOrderCorrect() {
    TEST("Key ordering: key != value ordering")
    AVLTree<int, int> tree;
    // keys: 1 < 2 < 3, values: 30 > 20 > 10 (opposite order)
    tree.Insert({1, 30});
    tree.Insert({2, 20});
    tree.Insert({3, 10});
    VerifyBalanceAndNodeCount(tree, 3);
    std::cout << std::endl << "  InOrder: ";
    tree.InOrder();
    // Expected: 1:30 2:20 3:10 (sorted by KEY)
    END_TEST
}

// ---- Different Types ----

void TestIntKeyDoubleValue() {
    TEST("int key, double value + balance")
    AVLTree<int, double> tree;
    assert(tree.Insert({1, 3.14}));
    assert(tree.Insert({3, 1.59}));
    assert(tree.Insert({2, 2.71}));
    assert(tree.Insert({5, 0.99}));
    assert(tree.Insert({4, 4.0}));
    VerifyBalanceAndNodeCount(tree, 5);
    std::cout << std::endl << "  InOrder: ";
    tree.InOrder();
    END_TEST
}

void TestStringKeyIntValue() {
    TEST("string key, int value (lexicographic order)")
    AVLTree<std::string, int> tree;
    assert(tree.Insert({"date",   4}));
    assert(tree.Insert({"apple",  2}));
    assert(tree.Insert({"cherry", 3}));
    assert(tree.Insert({"banana", 1}));
    VerifyBalanceAndNodeCount(tree, 4);
    std::cout << std::endl
              << "  InOrder (expected: apple banana cherry date): ";
    tree.InOrder();
    END_TEST
}

// ---- Edge Cases ----

void TestAllDuplicates() {
    TEST("All duplicate keys -- only first succeeds")
    AVLTree<int, int> tree;
    assert(tree.Insert({42, 1}));
    for (int i = 0; i < 20; ++i)
        assert(!tree.Insert({42, i}));
    VerifyBalanceAndNodeCount(tree, 1);
    END_TEST
}

void TestNegativeKeys() {
    TEST("Negative keys + balance")
    AVLTree<int, int> tree;
    assert(tree.Insert({-5, -5}));
    assert(tree.Insert({ 0,  0}));
    assert(tree.Insert({ 5,  5}));
    assert(tree.Insert({-3, -3}));
    assert(tree.Insert({ 3,  3}));
    assert(tree.Insert({-1, -1}));
    assert(tree.Insert({ 1,  1}));
    VerifyBalanceAndNodeCount(tree, 7);
    std::cout << std::endl << "  InOrder: ";
    tree.InOrder();
    END_TEST
}

void TestTwoNodes() {
    TEST("Two-node trees")
    AVLTree<int, int> t1;
    assert(t1.Insert({2, 2}));
    assert(t1.Insert({1, 1}));
    VerifyBalanceAndNodeCount(t1, 2);

    AVLTree<int, int> t2;
    assert(t2.Insert({1, 1}));
    assert(t2.Insert({2, 2}));
    VerifyBalanceAndNodeCount(t2, 2);
    END_TEST
}

// ---- Stress Tests ----

void TestStressSequential200() {
    TEST("Stress: sequential 200 nodes + balance")
    AVLTree<int, int> tree;
    for (int i = 1; i <= 200; ++i)
        assert(tree.Insert({i, i}));
    VerifyBalanceAndNodeCount(tree, 200);
    END_TEST
}

void TestStressReverse200() {
    TEST("Stress: reverse 200 nodes + balance")
    AVLTree<int, int> tree;
    for (int i = 200; i >= 1; --i)
        assert(tree.Insert({i, i}));
    VerifyBalanceAndNodeCount(tree, 200);
    END_TEST
}

void TestStressRandom500() {
    TEST("Stress: random 500 nodes + balance")
    AVLTree<int, int> tree;
    std::srand(12345);
    int count = 0;
    for (int i = 0; i < 500; ++i) {
        int v = std::rand() % 10000;
        if (tree.Insert({v, v})) ++count;
    }
    VerifyBalanceAndNodeCount(tree, count);
    std::cout << std::endl << "  Unique nodes inserted: " << count << std::endl;
    END_TEST
}

// ---- InOrder & Height ----

void TestInOrderSorted() {
    TEST("InOrder sorted verification (30 nodes)")
    AVLTree<int, int> tree;
    for (int i = 1; i <= 30; ++i)
        tree.Insert({i, i});
    VerifyBalanceAndNodeCount(tree, 30);

    auto* root = AVLTreeAccessor<int, int>::GetRoot(tree);
    std::vector<std::pair<int, int>> data;
    CollectInOrder(root, data);

    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i].first < data[i - 1].first)
            throw std::runtime_error(
                "InOrder not ascending: "
                + std::to_string(data[i - 1].first) + " > "
                + std::to_string(data[i].first));
    }
    std::cout << std::endl << "  Traversal: 1..30 strictly ascending" << std::endl;
    END_TEST
}

void TestTreeHeight() {
    TEST("AVL height bound: 200 nodes")
    AVLTree<int, int> tree;
    for (int i = 1; i <= 200; ++i)
        assert(tree.Insert({i, i}));
    VerifyBalanceAndNodeCount(tree, 200);
    auto* root = AVLTreeAccessor<int, int>::GetRoot(tree);
    int height = CheckBalance(root);
    std::cout << std::endl
              << "  200-node AVL tree height: " << height
              << " (theoretical upper bound ~= 12)" << std::endl;
    if (height > 12)
        throw std::runtime_error("Tree height exceeds AVL theoretical bound");
    END_TEST
}

// =============================================================
// main
// =============================================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  AVLTree Test Suite v3" << std::endl;
    std::cout << "  (balance + node count verified)" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    std::cout << "---- Basic ----" << std::endl;
    TestEmptyTree();
    TestSingleInsert();
    TestDuplicateKey();
    TestMultipleInsertsNoRotation();
    TestNodeCount();

    std::cout << std::endl << "---- Rotations ----" << std::endl;
    TestLLRotation();
    TestRRRotation();
    TestLRRotation();
    TestRLRotation();
    TestLLRotationExtended();
    TestRRRotationExtended();
    TestMixedRotations();
    TestLRRotationExtended();
    TestRLRotationExtended();

    std::cout << std::endl << "---- Large Scale ----" << std::endl;
    TestLargeSequential100();
    TestLargeReverse100();
    TestRandomInsert();

    std::cout << std::endl << "---- Key Ordering ----" << std::endl;
    TestKeyOrderCorrect();

    std::cout << std::endl << "---- Different Types ----" << std::endl;
    TestIntKeyDoubleValue();
    TestStringKeyIntValue();

    std::cout << std::endl << "---- Edge Cases ----" << std::endl;
    TestAllDuplicates();
    TestNegativeKeys();
    TestTwoNodes();

    std::cout << std::endl << "---- Stress Tests ----" << std::endl;
    TestStressSequential200();
    TestStressReverse200();
    TestStressRandom500();

    std::cout << std::endl << "---- InOrder & Height ----" << std::endl;
    TestInOrderSorted();
    TestTreeHeight();

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "  Result: " << g_passed << " passed, "
              << g_failed << " failed (total " << (g_passed + g_failed) << ")" << std::endl;
    std::cout << "========================================" << std::endl;

    return g_failed > 0 ? 1 : 0;
}
