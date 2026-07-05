#if 1

#include<iostream>
#include<string>
#include<vector>
#include<random>
#include"RBTree.h"

int g_testsPassed = 0;
int g_testsFailed = 0;

#define TEST(name) \
	std::cout << "=== " << name << " ===" << std::endl;

#define CHECK(cond, msg) \
	do { \
		if (cond) { \
			std::cout << "  [PASS] " << msg << std::endl; \
			++g_testsPassed; \
		} else { \
			std::cout << "  [FAIL] " << msg << std::endl; \
			++g_testsFailed; \
		} \
	} while(0)

// ============================================================================
// Test 1: empty tree
// ============================================================================
void TestEmptyTree() {
	TEST("Empty Tree")
	RBTree<int, int> rbt;
	CHECK(rbt.Size() == 0, "size == 0");
	CHECK(rbt.IsValidRBTree(), "valid RB tree");
	CHECK(rbt.Find(1) == nullptr, "find on empty returns nullptr");
}

// ============================================================================
// Test 2: single node (root must be black)
// ============================================================================
void TestSingleNode() {
	TEST("Single Node")
	RBTree<int, int> rbt;
	rbt.Insert({42, 100});
	CHECK(rbt.Size() == 1, "size == 1");
	CHECK(rbt.IsValidRBTree(), "valid RB tree");
	CHECK(rbt.Find(42) != nullptr, "find existing key");
	CHECK(*rbt.Find(42) == 100, "value correct");
	CHECK(rbt.Find(99) == nullptr, "find non-existing key");
}

// ============================================================================
// Test 3: duplicate keys are rejected
// ============================================================================
void TestDuplicateKeys() {
	TEST("Duplicate Key Rejection")
	RBTree<int, int> rbt;
	rbt.Insert({5, 10});
	rbt.Insert({5, 20});  // duplicate key, should be ignored
	rbt.Insert({5, 30});  // duplicate again
	CHECK(rbt.Size() == 1, "size stays 1 after duplicates");
	CHECK(*rbt.Find(5) == 10, "original value preserved");
	CHECK(rbt.IsValidRBTree(), "valid RB tree");
}

// ============================================================================
// Test 4: ascending order insertion (triggers RR rotations + recolor)
// ============================================================================
void TestAscendingInsert() {
	TEST("Ascending Order Insert (1..20)")
	RBTree<int, int> rbt;
	for (int i = 1; i <= 20; ++i) {
		rbt.Insert({i, i * 10});
	}
	CHECK(rbt.Size() == 20, "size == 20");
	CHECK(rbt.IsValidRBTree(), "valid RB tree");
	// All keys must be findable
	bool allFound = true;
	for (int i = 1; i <= 20; ++i) {
		auto* v = rbt.Find(i);
		if (!v || *v != i * 10) { allFound = false; break; }
	}
	CHECK(allFound, "all 20 keys found with correct values");

	std::cout << "  "; rbt.PrintInOrder();
}

// ============================================================================
// Test 5: descending order insertion (triggers LL rotations)
// ============================================================================
void TestDescendingInsert() {
	TEST("Descending Order Insert (20..1)")
	RBTree<int, int> rbt;
	for (int i = 20; i >= 1; --i) {
		rbt.Insert({i, i * 10});
	}
	CHECK(rbt.Size() == 20, "size == 20");
	CHECK(rbt.IsValidRBTree(), "valid RB tree");

	bool allFound = true;
	for (int i = 1; i <= 20; ++i) {
		auto* v = rbt.Find(i);
		if (!v || *v != i * 10) { allFound = false; break; }
	}
	CHECK(allFound, "all 20 keys found with correct values");
	std::cout << "  "; rbt.PrintInOrder();
}

// ============================================================================
// Test 6: zig-zag insertion (triggers LR / RL double rotations)
// ============================================================================
void TestZigZagInsert() {
	TEST("Zig-Zag Insert (triggers LR/RL rotations)")
	// Insert pattern: 10, 5, 8  =>  LR rotation
	// Insert pattern: 10, 15, 12 => RL rotation
	RBTree<int, int> rbt;

	// LR case
	rbt.Insert({10, 0});
	rbt.Insert({5, 0});
	rbt.Insert({8, 0});   // triggers LR: left-rotate(5) then right-rotate(10)

	// RL case
	rbt.Insert({15, 0});
	rbt.Insert({12, 0});  // triggers RL: right-rotate(15) then left-rotate(10)

	CHECK(rbt.Size() == 5, "size == 5");
	CHECK(rbt.IsValidRBTree(), "valid RB tree after LR and RL rotations");

	// Also test LR in isolation
	{
		RBTree<int, int> t2;
		t2.Insert({50, 0});
		t2.Insert({30, 0});
		t2.Insert({40, 0});  // LR
		CHECK(t2.IsValidRBTree(), "isolated LR case valid");
		CHECK(t2.Size() == 3, "isolated LR size == 3");
	}

	// Also test RL in isolation
	{
		RBTree<int, int> t3;
		t3.Insert({50, 0});
		t3.Insert({70, 0});
		t3.Insert({60, 0});  // RL
		CHECK(t3.IsValidRBTree(), "isolated RL case valid");
		CHECK(t3.Size() == 3, "isolated RL size == 3");
	}

	std::cout << "  "; rbt.PrintInOrder();
}

// ============================================================================
// Test 7: uncle-is-red recolor case
// ============================================================================
void TestRecolorCase() {
	TEST("Uncle-Red Recolor")
	// Inserting in a pattern that triggers the recolor (uncle is red)
	// Insert 10, then 5 and 15 -> both children red, then insert 3
	// uncle(15) is red => recolor father(5), uncle(15), grandpa(10)
	RBTree<int, int> rbt;
	rbt.Insert({10, 0});
	rbt.Insert({5, 0});
	rbt.Insert({15, 0});
	rbt.Insert({3, 0});   // triggers recolor of 5,15,10
	rbt.Insert({12, 0});
	rbt.Insert({18, 0});

	CHECK(rbt.Size() == 6, "size == 6");
	CHECK(rbt.IsValidRBTree(), "valid RB tree after recolor");
	std::cout << "  "; rbt.PrintInOrder();
}

// ============================================================================
// Test 8: string keys
// ============================================================================
void TestStringKeys() {
	TEST("String Keys")
	RBTree<std::string, int> rbt;
	rbt.Insert({"delta",    4});
	rbt.Insert({"charlie",  3});
	rbt.Insert({"alpha",    1});
	rbt.Insert({"echo",     5});
	rbt.Insert({"bravo",    2});
	rbt.Insert({"foxtrot",  6});

	CHECK(rbt.Size() == 6, "size == 6");
	CHECK(rbt.IsValidRBTree(), "valid RB tree with string keys");

	CHECK(*rbt.Find("alpha")    == 1, "alpha == 1");
	CHECK(*rbt.Find("bravo")    == 2, "bravo == 2");
	CHECK(*rbt.Find("charlie")  == 3, "charlie == 3");
	CHECK(*rbt.Find("delta")    == 4, "delta == 4");
	CHECK(*rbt.Find("echo")     == 5, "echo == 5");
	CHECK(*rbt.Find("foxtrot")  == 6, "foxtrot == 6");
	CHECK(rbt.Find("zulu") == nullptr, "non-existing key returns nullptr");

	std::cout << "  "; rbt.PrintInOrder();
}

// ============================================================================
// Test 9: mixed insert + validate continuously
// ============================================================================
void TestContinuousValidation() {
	TEST("Continuous Validation (insert 100 ints, validate every 10)")
	RBTree<int, int> rbt;
	std::vector<int> keys;
	for (int i = 0; i < 100; ++i) keys.push_back(i);

	// Shuffle with deterministic seed
	std::mt19937 rng(42);
	std::shuffle(keys.begin(), keys.end(), rng);

	bool allValid = true;
	for (int i = 0; i < 100; ++i) {
		rbt.Insert({keys[i], keys[i]});
		if ((i + 1) % 10 == 0) {
			if (!rbt.IsValidRBTree()) {
				allValid = false;
				std::cout << "  Validation failed at i=" << i << std::endl;
				break;
			}
		}
	}
	CHECK(allValid, "RB tree valid after every 10 insertions");
	CHECK(rbt.Size() == 100, "size == 100");

	// Verify 100 random lookups
	bool allOk = true;
	for (int i = 0; i < 100; ++i) {
		auto* v = rbt.Find(i);
		if (!v || *v != i) { allOk = false; break; }
	}
	CHECK(allOk, "all 100 keys found, values match");
}

// ============================================================================
// Test 10: stress test — 1000 random insertions
// ============================================================================
void TestStressLarge() {
	TEST("Stress Test (1000 random insertions)")
	RBTree<int, int> rbt;
	std::vector<int> keys;
	for (int i = 0; i < 1000; ++i) keys.push_back(i);

	std::mt19937 rng(12345);
	std::shuffle(keys.begin(), keys.end(), rng);

	for (int i = 0; i < 1000; ++i) {
		rbt.Insert({keys[i], keys[i] * 10});
	}
	CHECK(rbt.Size() == 1000, "size == 1000");
	CHECK(rbt.IsValidRBTree(), "valid RB tree after 1000 insertions");

	// Spot-check some values
	bool ok = true;
	for (int i = 0; i < 1000; i += 137) {
		auto* v = rbt.Find(i);
		if (!v || *v != i * 10) { ok = false; break; }
	}
	CHECK(ok, "spot-check values correct");
}

// ============================================================================
// Test 11: RR rotation case isolated
// ============================================================================
void TestRRRotation() {
	TEST("RR Rotation (isolated)")
	// Insert 1,2,3: 2 becomes root, 1 left child, 3 right child
	RBTree<int, int> rbt;
	rbt.Insert({1, 0});
	rbt.Insert({2, 0});
	rbt.Insert({3, 0});  // triggers RR -> left rotation at 1
	CHECK(rbt.Size() == 3, "size == 3");
	CHECK(rbt.IsValidRBTree(), "valid after RR rotation");
}

// ============================================================================
// Test 12: LL rotation case isolated
// ============================================================================
void TestLLRotation() {
	TEST("LL Rotation (isolated)")
	// Insert 3,2,1: 2 becomes root, 1 left child, 3 right child
	RBTree<int, int> rbt;
	rbt.Insert({3, 0});
	rbt.Insert({2, 0});
	rbt.Insert({1, 0});  // triggers LL -> right rotation at 3
	CHECK(rbt.Size() == 3, "size == 3");
	CHECK(rbt.IsValidRBTree(), "valid after LL rotation");
}

// ============================================================================
int main() {
	std::cout << "================================" << std::endl;
	std::cout << "  Red-Black Tree Unit Tests" << std::endl;
	std::cout << "================================" << std::endl << std::endl;

	TestEmptyTree();
	std::cout << std::endl;

	TestSingleNode();
	std::cout << std::endl;

	TestDuplicateKeys();
	std::cout << std::endl;

	TestAscendingInsert();
	std::cout << std::endl;

	TestDescendingInsert();
	std::cout << std::endl;

	TestZigZagInsert();
	std::cout << std::endl;

	TestRecolorCase();
	std::cout << std::endl;

	TestStringKeys();
	std::cout << std::endl;

	TestContinuousValidation();
	std::cout << std::endl;

	TestStressLarge();
	std::cout << std::endl;

	TestRRRotation();
	std::cout << std::endl;

	TestLLRotation();
	std::cout << std::endl;

	std::cout << "================================" << std::endl;
	std::cout << "  Results: " << g_testsPassed << " passed, "
	          << g_testsFailed << " failed" << std::endl;
	std::cout << "================================" << std::endl;

	return g_testsFailed > 0 ? 1 : 0;
}
#endif