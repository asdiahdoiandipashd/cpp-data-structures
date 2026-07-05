#include<iostream>
#include<map>
#include<vector>

using namespace std;

int main() {
	map<string, string> dict;
	pair<string, string> p("hello", "你好");

	dict.insert(p);

	dict.insert(pair<string, string>("apple", "苹果"));

	dict.insert(make_pair("first", "第一个"));

	dict.insert({ "second","第二个" });

	for (auto it : dict) {
		cout << it.first << ":" << it.second << endl;
	}

	return 0;
}