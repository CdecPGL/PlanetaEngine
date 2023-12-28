#pragma once

#include "boost/serialization/nvp.hpp"
#include "boost/serialization/split_free.hpp"

#include "data_container.hpp"
#include "data_container_impl.hpp"

/*
boost::serializeライブラリを用いてGameDataをシリアライズするための関数群。
できるだけboostライブラリの依存を下げるために、非侵入型を採用した。
ただしシリアライズのために、必要な関数は各クラスでfriend指定し、シリアライズで用いる関数を追加した。
*/
namespace boost::serialization {
	/*GameDataElement*/
	template <class Archive>
	void serialize(Archive &ar, plnt::util::data_container &data_container, unsigned int version) {
		split_free(ar, data_container, version);
	}

	template <class Archive>
	void save(Archive &ar, const plnt::util::data_container &data_container, unsigned int) {
		using namespace plnt::util;
		auto &dci = *data_container.impl_;
		//整数要素を保存
		ar & make_nvp("integer_elements", dci.integer_elements);
		//論理値要素を保存
		ar & make_nvp("boolean_elements", dci.boolean_elements);
		//文字列要素を保存
		ar & make_nvp("string_elements", dci.string_elements);
		//実数要素を保存
		ar & make_nvp("number_elements", dci.number_elements);
	}

	template <class Archive>
	void load(Archive &ar, plnt::util::data_container &data_container, unsigned int) {
		using namespace plnt::util;
		auto &dci = *data_container.impl_;
		//整数要素を読み込み
		ar & make_nvp("integer_elements", dci.integer_elements);
		//論理値要素を読み込み
		ar & make_nvp("boolean_elements", dci.boolean_elements);
		//文字列要素を読み込み
		ar & make_nvp("string_elements", dci.string_elements);
		//実数要素を読み込み
		ar & make_nvp("number_elements", dci.number_elements);
		//要素型マップを構築
		dci.construct_element_type_map();
	}
}
