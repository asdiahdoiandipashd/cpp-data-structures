#pragma once
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<utility>
#include<cmath>
#include<random>
using namespace std;



	enum DataState {
		EXIST,
		EMPTY,
		DELETE
	};
namespace open_locate {
	//仿函数，将负数，整数，浮点数转为key
	template<class K>
	struct HashFunc
	{
		size_t operator()(const K& data) {
			return size_t(data);
		}
	};

	//特化string
	template<>
	struct HashFunc<string>
	{
		size_t operator()(const string& data) {

			size_t hash = 0;
			for (auto& e : data) {
				//BKDR法
				hash += e;
				hash *= 131;
			}

			return hash;
		}
	};

	template<class K, class V>
	struct HashData {
		pair<K, V> _data;
		DataState _state = EMPTY;
	};

	template<class K, class V, class HashFunc = HashFunc<K>>
	class HashTable {
	public:
		//复用vector的迭代器
		using Table = vector<HashData<K, V>>;
		using Iterator = typename Table::iterator;

		Iterator begin() {
			return _tables.begin();
		}

		Iterator end() {
			return _tables.end();
		}

		HashTable()
			:_tables(__stl_next_prime(0))
			, _size(0)
			, _a(1)
			, _b(0)
		{
			ResetHashParameters();
		}

		size_t GetSize() {
			return _size;
		}

		inline unsigned long __stl_next_prime(unsigned long n)
		{
			// Note: assumes long is at least 32 bits.
			static const int __stl_num_primes = 28;
			static const unsigned long __stl_prime_list[__stl_num_primes] =
			{
				53,         97,         193,        389,        769,
				1543,       3079,       6151,       12289,      24593,
				49157,      98317,      196613,     393241,     786433,
				1572869,    3145739,    6291469,    12582917,   25165843,
				50331653,   100663319,  201326611,  402653189,  805306457,
				1610612741, 3221225473, 4294967291
			};

			const unsigned long* first = __stl_prime_list;
			const unsigned long* last = __stl_prime_list + __stl_num_primes;
			const unsigned long* pos = lower_bound(first, last, n);
			return pos == last ? *(last - 1) : *pos;
		}

		/*HashData<K, V>* Find(const K& data) {
			size_t hash0 = data % _tables.size();
			size_t hashi = hash0;
			size_t i = 1;

			while (_tables[hashi]._state == EXIST ) {
				//线性探测
				if (_tables[hashi]._data.first == data) {
					return &_tables[hashi];
				}
				hashi = (hash0 + i)%_tables.size();
				++i;
			}
			return nullptr;

		}
		*/

		size_t Hash(const K& data) {
			//除法散列
			HashFunc hf;
			size_t hash = hf(data);
			hash = hash ^ (hash >> 16);
			return hash;
		}

		HashData<K, V>* Find(const K& data) {
			size_t hash0 = GetHashIndex(data);

			size_t hashi = hash0;
			int i = 1;
			int flag = 1;

			while (_tables[hashi]._state == EXIST) {
				//二次探测
				if (_tables[hashi]._data.first == data) {
					return &_tables[hashi];
				}
				hashi = (hash0 + i * i * flag) % _tables.size();
				flag = (flag == 1 ? -1 : 1);
				++i;
			}
			return nullptr;
		}

		bool Insert(const pair<K, V> data) {
			//判断扩容
			if (static_cast<double>(_size) / _tables.size() > 0.5) {
				//扩容逻辑
				HashTable newht;
				newht._tables.resize(__stl_next_prime(_tables.size() + 1));
				newht.ResetHashParameters();

				for (auto e : _tables) {
					if (e._state == EXIST) {
						newht.Insert(e._data);
					}
				}
				_tables.swap(newht._tables);
				_size = newht._size;
				_a = newht._a;
				_b = newht._b;
			}

			//hash0：插入下标
			//hashi：临时探测变量
			//i：偏移

			//除法散列
			//size_t hash0 = Hash(data.first) % _tables.size();

			//乘法散列
			//double hash_pre = Hash(data.first) * 0.6180339887498949;
			//size_t hash0 = _tables._size * fmod(hash_pre,1.0);


			//全域散列法计算初始下标，冲突仍使用二次探测
			size_t hash0 = GetHashIndex(data.first);

			size_t hashi = hash0;
			int i = 1;
			int flag = 1;
			while (_tables[hashi]._state == EXIST) {
				//二次探测
				if (_tables[hashi]._data.first == data.first) {
					return false;
				}
				hashi = (hash0 + i * i * flag) % _tables.size();
				flag = flag == 1 ? -1 : 1;
				++i;
			}

			_tables[hashi]._data = data;
			_tables[hashi]._state = EXIST;

			++_size;

			return true;
		}


	private:
		void ResetHashParameters() {
			size_t P = __stl_next_prime(1 + _tables.size());

			std::mt19937_64 gen(std::random_device{}());
			std::uniform_int_distribution<size_t> distA(1, P - 1);
			std::uniform_int_distribution<size_t> distB(0, P - 1);

			_a = distA(gen);
			_b = distB(gen);
		}

		size_t GetHashIndex(const K& data) {
			size_t P = __stl_next_prime(1 + _tables.size());
			size_t M = _tables.size();
			size_t key = Hash(data) % P;

			return ((_a * key + _b) % P) % M;
		}

		Table _tables;
		size_t _size;
		size_t _a;
		size_t _b;
	};
}

namespace chain_locate {
	// 与开放定址法共用相同的默认散列规则
	template<class K>
	struct HashFunc
	{
		size_t operator()(const K& data) {
			return size_t(data);
		}
	};

	// string 使用 BKDR 散列
	template<>
	struct HashFunc<string>
	{
		size_t operator()(const string& data) {
			size_t hash = 0;
			for (auto& e : data) {
				hash += e;
				hash *= 131;
			}

			return hash;
		}
	};

	template<class K, class V>
	struct HashNode {
		pair<K, V> _data;
		HashNode<K, V>* _next;

		HashNode(const pair<K, V>& data)
			:_data(data)
			, _next(nullptr)
		{}
	};

	template<class K, class V, class HashFunc = HashFunc<K>>
	class HashTable {
	public:
		using Node = HashNode<K, V>;
		using Table = vector<Node*>;
		using Iterator = typename Table::iterator;

		HashTable()
			:_tables(__stl_next_prime(0), nullptr)
			, _size(0)
			, _a(1)
			, _b(0)
		{
			ResetHashParameters();
		}

		// TODO: 释放每个桶中链表的全部节点
		~HashTable() {
			for (size_t i = 0; i < _tables.size(); ++i) {
				Node* cur = _tables[i];
				while (cur) {
					Node* next = cur->_next;
					delete cur;
					cur = next;
				}
			}
		}

		Iterator begin() {
			return _tables.begin();
		}

		Iterator end() {
			return _tables.end();
		}
		size_t GetSize() {
			return _size;
		}

		// TODO: 由你实现链表定址法的核心查找逻辑
		Node* Find(const K& data) {
			size_t hash0 = GetHashIndex(data);
			Node* cur = _tables[hash0];

			while (cur != nullptr) {
				if (cur->_data.first == data) {
					return cur;
				}

				cur = cur->_next;
			}

			return nullptr;
		}

		bool Insert(const pair<K, V>& data) {
			//查重
			if (Find(data.first) != nullptr) {
				return false;
			}

			if (_size == _tables.size()) {
				//扩容
				HashTable newht;
				newht._tables.resize(_tables.size() + 1);

				for (size_t i = 0; i < _tables.size(); ++i) {
					Node* cur = _tables[i];
					while (cur) {
						Node* next = cur->_next;
						size_t newhash0 = newht.GetHashIndex(cur->_data.first);

						cur->_next = newht._tables[newhash0];
						newht._tables[newhash0] = cur;

						cur = next;
					}
					_tables[i] = nullptr;
				}

				_tables.swap(newht._tables);
				std::swap(_a, newht._a);
				std::swap(_b, newht._b);
			}

			size_t hash0 = GetHashIndex(data.first);
			Node* newnode = new Node(data);
			newnode->_next = _tables[hash0];
			_tables[hash0] = newnode;

			++_size;
			return true;
		}

		bool Erase(const K& data) {
			size_t hash0 = GetHashIndex(data);
			Node* prev = nullptr;
			Node* cur = _tables[hash0];
			Node* next = nullptr;

			while (cur) {
				if (cur->_data.first == data) {
					if (cur == _tables[hash0]) {
						//头删
						_tables[hash0] = next;
					}
					else{
						//中间节点
						prev->_next = next;
					}
					delete cur;
					cur == nullptr;
					return true;
				}

				prev = cur;
				cur = next;
				next = cur->_next;
			}
			return false;
		}

	private:
		inline unsigned long __stl_next_prime(unsigned long n)
		{
			static const int __stl_num_primes = 28;
			static const unsigned long __stl_prime_list[__stl_num_primes] =
			{
				53,         97,         193,        389,        769,
				1543,       3079,       6151,       12289,      24593,
				49157,      98317,      196613,     393241,     786433,
				1572869,    3145739,    6291469,    12582917,   25165843,
				50331653,   100663319,  201326611,  402653189,  805306457,
				1610612741, 3221225473, 4294967291
			};

			const unsigned long* first = __stl_prime_list;
			const unsigned long* last = __stl_prime_list + __stl_num_primes;
			const unsigned long* pos = lower_bound(first, last, n);
			return pos == last ? *(last - 1) : *pos;
		}

		size_t Hash(const K& data) {
			HashFunc hf;
			size_t hash = hf(data);
			hash = hash ^ (hash >> 16);
			return hash;
		}

		void ResetHashParameters() {
			size_t P = __stl_next_prime(1 + _tables.size());

			std::mt19937_64 gen(std::random_device{}());
			std::uniform_int_distribution<size_t> distA(1, P - 1);
			std::uniform_int_distribution<size_t> distB(0, P - 1);

			_a = distA(gen);
			_b = distB(gen);
		}

		size_t GetHashIndex(const K& data) {
			size_t P = __stl_next_prime(1 + _tables.size());
			size_t M = _tables.size();
			size_t key = Hash(data) % P;

			return ((_a * key + _b) % P) % M;
		}

		Table _tables;
		size_t _size;
		size_t _a;
		size_t _b;
	};
}
