#pragma once

#include <unordered_map>
#include <algorithm>
#include <initializer_list>
#include <tuple>

namespace planeta_engine {
	namespace util {
		/*Keyには大小比較演算子が定義されていて、Elemにはデフォルトコンストラクタが定義されている必要がある。*/
		template<typename Key, typename Elem>
		class TwoKeyHashMap final{
		public:
			TwoKeyHashMap() = default;
			/*TwoKeyHashMap(std::initializer_list<std::tuple<Key,Key,Elem>> il) {
				for (auto& i : il) {
					Set(std::get<0>(i), std::get<1>(i), std::get<2>(i));
				}
			}*/
			void Set(const Key& k1, const Key& k2, const Elem& elem) {
				auto ordered_key_pair = std::minmax(k1, k2);
				map_[ordered_key_pair.first][ordered_key_pair.second] = elem;
			}
			const Elem& Get(const Key& k1, const Key& k2,const Elem& default_value = Elem()){
				auto ordered_key_pair = std::minmax(k1, k2);
				auto itf = map_.find(ordered_key_pair.first);
				if (itf == map_.end()) {
					itf = map_.emplace(ordered_key_pair.first, std::unordered_map<Key,Elem>({ { ordered_key_pair.second,default_value } })).first;
					return itf->second.begin()->second;
				}
				else {
					auto its = itf->second.find(ordered_key_pair.second);
					if (its == itf->second.end()) {
						return itf->second.emplace(ordered_key_pair.second, default_value).first->second;
					}
					else {
						return its->second;
					}
				}
			}
			void Clear() { map_.clear(); }
		private:
			std::unordered_map<Key, std::unordered_map<Key, Elem>> map_;
		};
	}
}
