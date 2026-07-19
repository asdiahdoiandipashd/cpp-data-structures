#include"unordered_map.h"
#include"unordered_set.h"
#include"HashTable.h"

#include<assert.h>
#include<type_traits>


void TestConstIterator() {
	mystl::unordered_map<string, string> map;
	map.insert({ "apple", "red" });
	map.insert({ "banana", "yellow" });

	using Map = mystl::unordered_map<string, string>;
	Map::iterator it = map.begin();
	Map::const_iterator const_it = it;
	assert(it == const_it);
	assert(const_it == it);

	const Map& const_map = map;
	static_assert(is_same<decltype(*const_map.begin()), const Map::value_type&>::value,
		"const_iterator must return const value_type&");
	static_assert(is_assignable<decltype((map.begin()->second)), string>::value,
		"iterator must allow mapped value modification");
	static_assert(!is_assignable<decltype((const_map.begin()->second)), string>::value,
		"const_iterator must reject mapped value modification");

	size_t map_count = 0;
	for (auto pos = const_map.begin(); pos != const_map.end(); ++pos) {
		cout << pos->first << ":" << pos->second << endl;
		++map_count;
	}
	assert(map_count == 2);

	mystl::unordered_set<int> set;
	set.insert(1);
	set.insert(2);
	static_assert(is_same<decltype(*set.begin()), const int&>::value,
		"unordered_set iterator must not allow key modification");
	const mystl::unordered_set<int>& const_set = set;

	size_t set_count = 0;
	for (auto pos = const_set.begin(); pos != const_set.end(); ++pos) {
		++set_count;
	}
	assert(set_count == 2);
}



int main() {
	//TestConstIterator();
	mystl::unordered_map<string, string> um;
	um.insert({ "apple", "苹果" });
	um["apple"];
	return 0;
}
