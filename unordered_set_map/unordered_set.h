#pragma once
#include"HashTable.h"

namespace mystl {
	template<class K, class HashFunc = chain_locate::HashFunc<K>>
	class unordered_set {
		struct SetKeyOfT {
			const K& operator()(const K& key) const {
				return key;
			}
		};
	public:
		using key_type = K;
		using value_type = K;
		using table_type = chain_locate::HashTable<key_type, value_type, SetKeyOfT, HashFunc>;
		using iterator = typename table_type::ConstIterator;
		using const_iterator = typename table_type::ConstIterator;
		

		iterator begin() {
			return _ht.cbegin();
		}

		iterator end() {
			return _ht.cend();
		}

		const_iterator begin() const {
			return _ht.cbegin();
		}

		const_iterator end() const {
			return _ht.cend();
		}

		const_iterator cbegin() const {
			return _ht.cbegin();
		}

		const_iterator cend() const {
			return _ht.cend();
		}

		pair<iterator, bool> insert(const K& key) {
			return _ht.Insert(key);
		}

		bool erase(const K& key) {
			return _ht.Erase(key);
		}

	private:
		table_type _ht;
	};
}
