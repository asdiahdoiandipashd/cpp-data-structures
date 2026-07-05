#include"map.h"
#include"set.h"

int main() {
	/*
	//set²âÊÔ
	myset<std::string> ms;
	ms.Insert("13");
	ms.Insert("14");
	ms.Insert("15");
	ms.Insert("16");

	myset<std::string>::Iterator it = ms.begin();

	while (it != ms.end()) {
		std::cout << *it << " ";
		++it;
	}*/

	//map²âÊÔ
	mymap<std::string, std::string> mp;
	mp.Insert({ "apple", "Æ»¹û" });
	mp.Insert({ "banana", "Ïã½¶" });
	mp.Insert({ "abandon", "·ÅÆú" });

	mymap<std::string, std::string>::Iterator it1 = mp.begin();
	while (it1 != mp.end()) {
		std::cout << (*it1).first << ":"<<(*it1).second<<" ";
		++it1;
	}

	return 0;
}