#pragma once

#include "boost/serialization/serialization.hpp"
#include "boost/serialization/string.hpp"
#include "boost/serialization/unordered_map.hpp"
#include "boost/serialization/vector.hpp"

#include "GameDataElementType.h"
#include "GameDataError.h"
#include "GameDataElement.h"
#include "GameDataComplexTypeInstance.h"

/*
boost::serializeライブラリを用いてGameDataElementとGameDataComplexTypeInstanceをシリアライズするための関数群。
できるだけboostライブラリの依存を下げるために、非侵入型を採用した。
ただしシリアライズのために、必要な関数は各クラスでfriend指定し、シリアライズで用いる関数を追加した。
特にGameDataComplexTypeInstanceはGameDataComplexTypeの参照を保持するが、シリアライズ時に変更する必要があるためポインタで保持するようにしてある。
*/
namespace boost {
	namespace serialization {
		/*GameDataElement*/
		template<class Archive>
		void serialize(Archive& ar,planeta_engine::core::GameDataElement& game_data_element,unsigned int) {
			split_free(ar, game_data_element, version);
		}
		template<class Archive>
		void save(Archive& ar, planeta_engine::core::GameDataElement& game_data_element, unsigned int) {
			using namespace planeta_engine::core;
			//型IDを保存
			ar << game_data_element.type_id();
			//型の種類に応じて保存
			GameDataElementType::Type type = GameDataElementType::ConvertTypeIDToType(game_data_element.type_id());
			switch (type)
			{
			case planeta_engine::core::GameDataElementType::Type::int32_type:
				ar << game_data_element.GetInt32();
				break;
			case planeta_engine::core::GameDataElementType::Type::int64_type:
				ar << game_data_element.GetInt64();
				break;
			case planeta_engine::core::GameDataElementType::Type::bool_type:
				ar << game_data_element.GetBool();
				break;
			case planeta_engine::core::GameDataElementType::Type::double_type:
				ar << game_data_element.GetDouble();
				break;
			case planeta_engine::core::GameDataElementType::Type::string_type:
				ar << game_data_element.GetString();
				break;
			case planeta_engine::core::GameDataElementType::Type::array_type:
				ar << game_data_element.GetArray();
				break;
			case planeta_engine::core::GameDataElementType::Type::complex_type:
				ar << game_data_element.GetComplexType();
				break;
			default:
				assert(false);
				throw GameDataError("無効なGameDataです。");
				break;
			}
		}
		template<class Archive>
		void load(Archive& ar, planeta_engine::core::GameDataElement& game_data_element, unsigned int) {
			using namespace planeta_engine::core;
			//型IDを読み込み
			std::string type_id;
			ar >> type_id;
			game_data_element.SetType_for_boost_serialize_(type_id);
			//型の種類に応じて保読み込み
			GameDataElementType::Type type = GameDataElementType::ConvertTypeIDToType(type_id);
			switch (type)
			{
			case planeta_engine::core::GameDataElementType::Type::int32_type:
			{
				int32_t v;
				ar >> v;
				game_data_element.SetInt32(v);
			}
				break;
			case planeta_engine::core::GameDataElementType::Type::int64_type:
			{
				int64_t v;
				ar >> v;
				game_data_element.SetInt64(v);
			}
				break;
			case planeta_engine::core::GameDataElementType::Type::bool_type:
			{
				bool v;
				ar >> v;
				game_data_element.SetBool(v);
			}
				break;
			case planeta_engine::core::GameDataElementType::Type::double_type:
			{
				double v;
				ar >> v;
				game_data_element.SetDouble(v);
			}
				break;
			case planeta_engine::core::GameDataElementType::Type::string_type:
			{
				std::string v;
				ar >> v;
				game_data_element.SetString(std::move(v));
			}
				break;
			case planeta_engine::core::GameDataElementType::Type::array_type:
			{
				std::vector<GameDataElement> v;
				ar >> v;
				game_data_element.SetArray(std::move(v));
			}
			break;
			case planeta_engine::core::GameDataElementType::Type::complex_type:
			{
				GameDataComplexTypeInstance v;
				ar >> v;
				game_data_element.SetComplexType(std::move(v));
			}
				break;
			default:
				assert(false);
				throw GameDataError("無効なGameDataです。");
				break;
			}
		}
		/*GameComplexTypeInstance*/
		template<class Archive>
		void serialize(Archive& ar, planeta_engine::core::GameDataComplexTypeInstance& game_data_complex_type_instance, unsigned int) {
			split_free(ar, game_data_complex_type_instance, version);
		}
		template<class Archive>
		void save(Archive& ar, planeta_engine::core::GameDataComplexTypeInstance&  game_data_complex_type_instance, unsigned int) {
			//型IDを保存
			ar << game_data_complex_type_instance.complex_type().type_id();
			//要素を保存
			ar << game_data_complex_type_instance.elements_;
		}
		template<class Archive>
		void load(Archive& ar, planeta_engine::core::GameDataComplexTypeInstance&  game_data_complex_type_instance, unsigned int) {
			//型IDを読み込み
			std::string type_id;
			ar >> type_id;
			game_data_complex_type_instance.SetType_for_boost_serialize_(GameDataElementType::GetComplexType(type_id));
			//要素を読み込み
			ar >> game_data_complex_type_instance.elements_;
		}
	}
}