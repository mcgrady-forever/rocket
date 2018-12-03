#ifndef ROCKET_BASE_CONSISTENT_HASH_H_
#define ROCKET_BASE_CONSISTENT_HASH_H_

#include <assert.h>

#include <map>
#include <string>
#include <vector>

#include "murmurhash.h"

namespace rocket {
namespace base {

template<class T>
class ConsistentHash {
public:
	ConsistentHash(const std::vector<T>& real_nodes);

	~ConsistentHash();

	T get(const std::string& key);

private:
	void init();

private:
	enum {
		VIRTUAL_NODES_NUM = 50,
	};

	std::map<uint32_t, T>	_virtual_nodes;
	std::vector<T>			_shards;
};

template<class T>
ConsistentHash<T>::ConsistentHash(const std::vector<T>& real_nodes) {
	assert(real_nodes.size() > 0);
	_shards = real_nodes;
	init();
}

template<class T>
void ConsistentHash<T>::init() {
	for (int i = 0; i < _shards.size(); ++i) {
		for (int j = 0; j < VIRTUAL_NODES_NUM; ++j) {
			std::string key = "SHARD_" + std::to_string(i) + "_" + std::to_string(j);
			_virtual_nodes.insert(std::make_pair(murmurhash(key.c_str(), key.size()), _shards[i]));
		}
	}
}

template<class T>
T ConsistentHash<T>::get(const std::string& key) {
	auto it = _virtual_nodes.lower_bound(murmurhash(key.c_str(), key.size()));
	if (it != _virtual_nodes.end())
	{
		return it->second;
	}
	return _virtual_nodes.begin()->second;
}

template<class T>
ConsistentHash<T>::~ConsistentHash() {
	std::vector<T>().swap(_shards);
	_virtual_nodes.clear();
}

}  // namespace base
}  // namespace rocket

#endif // ROCKET_BASE_CONSISTENT_HASH_H_