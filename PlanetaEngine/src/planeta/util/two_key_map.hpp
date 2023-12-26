#pragma once

#include <unordered_map>
#include <algorithm>

namespace plnt::util {
	/*Keyには大小比較演算子が定義されていて、Elemにはデフォルトコンストラクタが定義されている必要がある。*/
	template <typename Key, typename Elem>
	class two_key_hash_map final {
	public:
		two_key_hash_map() = default;

		void set(const Key &k1, const Key &k2, const Elem &elem) {
			auto ordered_key_pair = std::minmax(k1, k2);
			map_[ordered_key_pair.first][ordered_key_pair.second] = elem;
		}

		const Elem &get(const Key &k1, const Key &k2, const Elem &default_value = Elem()) {
			auto ordered_key_pair = std::minmax(k1, k2);
			auto itf = map_.find(ordered_key_pair.first);
			if (itf == map_.end()) {
				itf = map_.emplace(ordered_key_pair.first,
				                   std::unordered_map<Key, Elem>({{ordered_key_pair.second, default_value}})).first;
				return itf->second.begin()->second;
			}
			auto its = itf->second.find(ordered_key_pair.second);
			if (its == itf->second.end()) {
				return itf->second.emplace(ordered_key_pair.second, default_value).first->second;
			}
			return its->second;
		}

		void clear() { map_.clear(); }

	private:
		std::unordered_map<Key, std::unordered_map<Key, Elem>> map_;
	};
}
