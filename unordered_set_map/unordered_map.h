#pragma once
#include"HashTable.h"

namespace mystl {
	template<class K, class V, class HashFunc =chain_locate::HashFunc<K>>
	class unordered_map {
		struct MapKeyOfT
		{
			const K& operator()(const std::pair<K,V>& data) const {
				return data.first;
			}
		};

	public:
		using key_type = K;
		using value_type = std::pair<K, V>;
		using table_type = chain_locate::HashTable<key_type, value_type, MapKeyOfT, HashFunc>;
		using iterator = typename table_type::Iterator;
		using const_iterator = typename table_type::ConstIterator;
		

		iterator begin() {
			return  _ht.begin();
		}

		iterator end() {
			return _ht.end();
		}

		const_iterator begin() const {
			return  _ht.begin();
		}

		const_iterator end() const {
			return _ht.end();
		}

		const_iterator cbegin() const {
			return _ht.cbegin();
		}

		const_iterator cend() const {
			return _ht.cend();
		}

		pair<iterator, bool> insert(const pair<K,V>& data) {
			return _ht.Insert(data);
		}

		bool erase(const K& data) {
			return _ht.Erase(data);
		}

		V& operator[](const K& data) {
			pair<iterator, bool> ret = _ht.Insert({data,V{}});
			return ret.first->second;
		}

	private:

		table_type _ht;
	};
}
