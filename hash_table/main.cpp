#include"HashTable.h"
#include<cassert>
#include<string>

int main() {
    vector<pair<string, string>> words = {
        {"apple", "苹果"},
        {"banana", "香蕉"},
        {"computer", "计算机"},
        {"dictionary", "字典"},
        {"iterator", "迭代器"},
        {"language", "语言"},
        {"table", "表格"},
        {"vector", "动态数组"}
    };

    chain_locate::HashTable<string, string> dict;

    // 1. 插入词条
    for (const auto& word : words) {
        dict.Insert(word);
    }

    // 2. 测试重复插入
    bool inserted = dict.Insert({ "apple", "苹果" });

    auto apple = dict.Find("apple");
    assert(apple != nullptr);
    assert(apple->_data.second == "苹果");
    assert(inserted == false);
    assert(dict.Find("orange") == nullptr);
    assert(dict.GetSize() == words.size());

    cout << "有效词条数：" << dict.GetSize() << endl;
    cout << "重复插入结果：" << inserted << endl;

    // 3. 使用当前的 vector 迭代器遍历
    size_t slots = 0;
    size_t exists = 0;

    for (auto it = dict.begin(); it != dict.end(); ++it) {
        ++slots;

        // *it 是当前桶的头节点指针
        auto cur = *it;

        while (cur != nullptr) {
            ++exists;

            cout << cur->_data.first
                << " , "
                << cur->_data.second
                << endl;

            cur = cur->_next;
        }
    }

    cout << "底层桶数：" << slots << endl;
    cout << "有效节点数：" << exists << endl;

    // 4. 验证扩容后仍能使用同一套散列规则查找
    chain_locate::HashTable<string, string> expandedDict;
    for (size_t i = 0; i < 80; ++i) {
        assert(expandedDict.Insert({ "word" + to_string(i), "value" + to_string(i) }));
    }

    for (size_t i = 0; i < 80; ++i) {
        auto result = expandedDict.Find("word" + to_string(i));
        assert(result != nullptr);
        assert(result->_data.second == "value" + to_string(i));
    }

    assert(expandedDict.GetSize() == 80);


    dict.Erase("apple");

	return 0;
}
