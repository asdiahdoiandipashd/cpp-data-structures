#if 0
#include<iostream>
#include"RBTree.h"

int main() {
	RBTree<std::string, std::string> rbt;
	rbt.Insert({ "apple","苹果" });
	rbt.Insert({ "balance","平衡" });

	if (rbt.IsBalance()) {
		std::cout << "平衡";
	}
	else
	{
		std::cout << "不平衡";
	}


	rbt.PrintInOrder();
	return 0;
}
#endif