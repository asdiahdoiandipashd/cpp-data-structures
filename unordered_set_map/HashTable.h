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
	
	template<class K>
	struct HashFunc
	{
		size_t operator()(const K& data) {
			return size_t(data);
		}
	};

	
	template<>
	struct HashFunc<string>
	{
		size_t operator()(const string& data) {

			size_t hash = 0;
			for (auto& e : data) {
				//BKDR
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
		using Table = vector<HashData<K, V>>;
		using Iterator = typename Table::iterator;
		using Self = HashTable<K,V,HashFunc>


		HashTable()
			:_tables(__stl_next_prime(0))
			, _size(0)
			, _a(1)
			, _b(0)
		{
			ResetHashParameters();
		}

		HashTable(Self&& obj) {
			swap(obj);
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

		inline size_t __stl_next_prime(size_t n)
		{
			// Note: assumes long is at least 32 bits.
			static const int __stl_num_primes = 28;
			static const size_t __stl_prime_list[__stl_num_primes] =
			{
				53,         97,         193,        389,        769,
				1543,       3079,       6151,       12289,      24593,
				49157,      98317,      196613,     393241,     786433,
				1572869,    3145739,    6291469,    12582917,   25165843,
				50331653,   100663319,  201326611,  402653189,  805306457,
				1610612741, 3221225473, 4294967291
			};

			const size_t* first = __stl_prime_list;
			const size_t* last = __stl_prime_list + __stl_num_primes;
			const size_t* pos = lower_bound(first, last, n);
			return pos == last ? *(last - 1) : *pos;
		}

		//线性探测
		/*HashData<K, V>* Find(const K& data) {
			size_t hash0 = data % _tables.size();
			size_t hashi = hash0;
			size_t i = 1;

			while (_tables[hashi]._state == EXIST ) {
				
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
			
			if (static_cast<double>(_size) / _tables.size() > 0.5) {
				
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



			//乘法收敛
			//double hash_pre = Hash(data.first) * 0.6180339887498949;
			//size_t hash0 = _tables._size * fmod(hash_pre,1.0);


			//全域收敛
			size_t hash0 = GetHashIndex(data.first);

			size_t hashi = hash0;
			int i = 1;
			int flag = 1;
			while (_tables[hashi]._state == EXIST) {
				//双重探测
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
	template<class K>
	struct HashFunc
	{
		size_t operator()(const K& data) {
			return size_t(data);
		}
	};

	//特化
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

	template<class T>
	struct HashNode {
		T _data;
		HashNode<T>* _next;

		HashNode(const T& data)
			:_data(data)
			, _next(nullptr)
		{}
	};

	template<class K, class T, class KeyOfT, class HashFunc>
	class HashTable;

	//迭代器
	template<class K, class T, class Ref, class Ptr, class KeyOfT, class HashFunc>
	struct HTIterator
	{
	public:
		using Node = HashNode<T>;
		using Table = HashTable<K, T, KeyOfT, HashFunc>;
		using Self = HTIterator<K, T, Ref, Ptr, KeyOfT, HashFunc>;

		Node* _node;
		const Table* _ht;
		KeyOfT kot;

	public:
		HTIterator(Node* node, const Table* ht)
			:_node(node)
			, _ht(ht)
		{
		}

		// 普通迭代器可以隐式转换为 const 迭代器，反向转换不允许。
		HTIterator(const HTIterator<K, T, T&, T*, KeyOfT, HashFunc>& it)
			:_node(it._node)
			, _ht(it._ht)
		{
		}

		Ref operator*() const {
			return (_node->_data);
		}

		Ptr operator->() const {
			return &(_node->_data);
		}

		Self& operator++() {
			Node* cur = _node;
			if (cur->_next) {
				_node = _node->_next;
			}
			else {
				size_t hashi = _ht->GetHashIndex(kot(_node->_data)) + 1;//原有哈希桶结束，进入下一个桶所以+1
				for (size_t i = hashi; i < _ht->_tables.size(); ++i) {
					if (_ht->_tables[i]) {
						_node = _ht->_tables[i];
						return *this;
					}
				}
				_node = nullptr;
			}
			return *this;
		}

		template<class Ref2, class Ptr2>
		bool operator!=(const HTIterator<K, T, Ref2, Ptr2, KeyOfT, HashFunc>& data) const {
			return _node != data._node;
		}

		template<class Ref2, class Ptr2>
		bool operator==(const HTIterator<K, T, Ref2, Ptr2, KeyOfT, HashFunc>& data) const
		{
			return _node == data._node;
		}
	};
	
	// K:key类型  T:节点实际存储的数据类型(set为K，map为pair<K,V>)
	// KeyOfT:从T中取出key的仿函数(SetKeyOfT/MapKeyOfT)
	template<class K, class T, class KeyOfT, class HashFunc>
	class HashTable {
	public:
		using Node = HashNode<T>;
		using Table = vector<Node*>;
		// friend：让迭代器能访问 _tables 和 GetHashIndex
		template<class K1, class T1, class Ref, class Ptr, class KeyOfT1, class HashFunc1>
		friend struct HTIterator;
	public:
		using Iterator = HTIterator<K, T, T&, T*, KeyOfT, HashFunc>;
		using ConstIterator = HTIterator<K, T, const T&, const T*, KeyOfT, HashFunc>;

		HashTable()
			:_tables(__stl_next_prime(0), nullptr)
			, _size(0)
			, _a(1)
			, _b(0)
		{
			ResetHashParameters();
		}

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
			size_t hashi = 0;

			for (size_t i = hashi; i < _tables.size(); ++i) {
				if (_tables[i]) {
					return Iterator(_tables[i],this);
				}
			}
			return end();
		}

		Iterator end() {
			return Iterator(nullptr, this);
		}

		ConstIterator begin()const {
			size_t hashi = 0;

			for (size_t i = hashi; i < _tables.size(); ++i) {
				if (_tables[i]) {
					return ConstIterator(_tables[i], this);
				}
			}
			return end();
		}

		ConstIterator end()const {
			return ConstIterator(nullptr, this);
		}

		ConstIterator cbegin() const {
			return begin();
		}

		ConstIterator cend() const {
			return end();
		}

		size_t GetSize() const {
			return _size;
		}

		// TODO: 链地址法的核心查找逻辑
		Iterator Find(const K& key) {
			KeyOfT kot;
			size_t hash0 = GetHashIndex(key);
			Node* cur = _tables[hash0];

			while (cur != nullptr) {
				if (kot(cur->_data) == key) {
					return Iterator(cur,this);
				}

				cur = cur->_next;
			}
		
			return end();
		}

		pair<Iterator, bool> Insert(const T& data) {
			KeyOfT kot;
			//查重
			Iterator ret = Find(kot(data));

			if (ret != end()) {
				return {ret,false};
			}

			if (_size == _tables.size()) {
				//扩容
				HashTable newht;
				newht._tables.resize(_tables.size() + 1);

				for (size_t i = 0; i < _tables.size(); ++i) {
					Node* cur = _tables[i];
					while (cur) {
						Node* next = cur->_next;
						size_t newhash0 = newht.GetHashIndex(kot(cur->_data));

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

			size_t hash0 = GetHashIndex(kot(data));
			Node* newnode = new Node(data);
			newnode->_next = _tables[hash0];
			_tables[hash0] = newnode;

			++_size;
			return { Iterator(newnode,this), true};
		}

		bool Erase(const K& key) {
			KeyOfT kot;
			size_t hash0 = GetHashIndex(key);
			Node* prev = nullptr;
			Node* cur = _tables[hash0];
			Node* next = nullptr;

			while (cur) {
				if (kot(cur->_data) == key) {
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

		inline size_t __stl_next_prime(size_t n) const
		{
			static const int __stl_num_primes = 28;
			static const size_t __stl_prime_list[__stl_num_primes] =
			{
				53,         97,         193,        389,        769,
				1543,       3079,       6151,       12289,      24593,
				49157,      98317,      196613,     393241,     786433,
				1572869,    3145739,    6291469,    12582917,   25165843,
				50331653,   100663319,  201326611,  402653189,  805306457,
				1610612741, 3221225473, 4294967291
			};

			const size_t* first = __stl_prime_list;
			const size_t* last = __stl_prime_list + __stl_num_primes;
			const size_t* pos = lower_bound(first, last, n);
			return pos == last ? *(last - 1) : *pos;
		}

		size_t Hash(const K& data) const {
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

		size_t GetHashIndex(const K& data) const {
			size_t P = __stl_next_prime(1 + _tables.size());
			size_t M = _tables.size();
			size_t key = Hash(data) % P;

			return ((_a * key + _b) % P) % M;
		}

	private:
		Table _tables;
		size_t _size;
		size_t _a;
		size_t _b;
	};

	
}

