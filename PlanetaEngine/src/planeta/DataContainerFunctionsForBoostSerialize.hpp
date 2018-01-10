#pragma once

#include "boost/serialization/serialization.hpp"
#include "boost/serialization/string.hpp"
#include "boost/serialization/unordered_map.hpp"
#include "boost/serialization/level.hpp"
#include "boost/serialization/nvp.hpp"
#include "boost/serialization/split_free.hpp"

#include "DataContainerError.hpp"
#include "DataContainer.hpp"
#include "DataContainerImpl.hpp"

/*
boost::serializeライブラリを用いてGameDataをシリアライズするための関数群。
できるだけboostライブラリの依存を下げるために、非侵入型を採用した。
ただしシリアライズのために、必要な関数は各クラスでfriend指定し、シリアライズで用いる関数を追加した。
*/
namespace boost {
	namespace serialization {
		/*GameDataElement*/
		template<class Archive>
		void serialize(Archive& ar, plnt::util::DataContainer& data_container, unsigned int version) {
			split_free(ar, data_container, version);
		}
		template<class Archive>
		void save(Archive& ar, const plnt::util::DataContainer& data_container, unsigned int) {
			using namespace plnt::util;
			auto& dci = *data_container.impl_;
			//整数要素を保存
			ar & make_nvp("integer_elements", dci.integer_elements_);
			//論理値要素を保存
			ar & make_nvp("boolean_elements", dci.boolean_elements_);
			//文字列要素を保存
			ar & make_nvp("string_elements", dci.string_elements_);
			//実数要素を保存
			ar & make_nvp("number_elements", dci.number_elements_);
		}
		template<class Archive>
		void load(Archive& ar, plnt::util::DataContainer& data_container, unsigned int) {
			using namespace plnt::util;
			auto& dci = *data_container.impl_;
			//整数要素を読み込み
			ar & make_nvp("integer_elements", dci.integer_elements_);
			//論理値要素を読み込み
			ar & make_nvp("boolean_elements", dci.boolean_elements_);
			//文字列要素を読み込み
			ar & make_nvp("string_elements", dci.string_elements_);
			//実数要素を読み込み
			ar & make_nvp("number_elements", dci.number_elements_);
			//要素型マップを構築
			dci.ConstructElementTypeMap();
		}
	}
}