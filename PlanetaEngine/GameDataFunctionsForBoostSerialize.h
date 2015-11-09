#pragma once

#include "boost/serialization/serialization.hpp"
#include "boost/serialization/string.hpp"
#include "boost/serialization/unordered_map.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/serialization/level.hpp"
#include "boost/serialization/nvp.hpp"
#include "boost/serialization/split_free.hpp"

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
		void serialize(Archive& ar,planeta_engine::core::GameDataElement& game_data_element,unsigned int version) {
			split_free(ar, game_data_element, version);
		}
		template<class Archive>
		void save(Archive& ar, const planeta_engine::core::GameDataElement& game_data_element, unsigned int) {
			using namespace planeta_engine::core;
			//型IDを保存
			ar & make_nvp("type_id", game_data_element.type_id());
			//型の種類に応じて保存
			GameDataElementType::Type type = GameDataElementType::ConvertTypeIDToType(game_data_element.type_id());
			int32_t int32_data = game_data_element.GetInt32();
			int64_t int64_data = game_data_element.GetInt64();
			bool bool_data = game_data_element.GetBool();
			double double_data = game_data_element.GetDouble();
			std::string& string_data = const_cast<std::string&>(game_data_element.GetString());
			std::vector<GameDataElement>& array_data = const_cast<std::vector<GameDataElement>&>(game_data_element.GetArray());
			GameDataComplexTypeInstance& complex_data = const_cast<GameDataComplexTypeInstance&>(game_data_element.GetComplexType());
			switch (type)
			{
			case planeta_engine::core::GameDataElementType::Type::int32_type:
				ar & make_nvp("int32_data", int32_data);
				break;
			case planeta_engine::core::GameDataElementType::Type::int64_type:
				ar & make_nvp("int64_data", int64_data);
				break;
			case planeta_engine::core::GameDataElementType::Type::bool_type:
				ar & make_nvp("bool_data", bool_data);
				break;
			case planeta_engine::core::GameDataElementType::Type::double_type:
				ar & make_nvp("double_data", double_data);
				break;
			case planeta_engine::core::GameDataElementType::Type::string_type:
				ar & make_nvp("string_data", string_data);
				break;
			case planeta_engine::core::GameDataElementType::Type::array_type:
				ar & make_nvp("array_data", array_data);
				break;
			case planeta_engine::core::GameDataElementType::Type::complex_type:
				ar & make_nvp("complex_data", complex_data);
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
			ar & make_nvp("type_id", type_id);
			game_data_element.SetType_for_boost_serialize_(type_id);
			//型の種類に応じて保読み込み
			GameDataElementType::Type type = GameDataElementType::ConvertTypeIDToType(type_id);
			switch (type)
			{
			case planeta_engine::core::GameDataElementType::Type::int32_type:
			{
				int32_t v;
				ar & make_nvp("int32_data", v);
				game_data_element.SetInt32(v);
			}
				break;
			case planeta_engine::core::GameDataElementType::Type::int64_type:
			{
				int64_t v;
				ar & make_nvp("int64_data", v);;
				game_data_element.SetInt64(v);
			}
				break;
			case planeta_engine::core::GameDataElementType::Type::bool_type:
			{
				bool v;
				ar & make_nvp("bool_data", v);;
				game_data_element.SetBool(v);
			}
				break;
			case planeta_engine::core::GameDataElementType::Type::double_type:
			{
				double v;
				ar & make_nvp("double_data", v);;
				game_data_element.SetDouble(v);
			}
				break;
			case planeta_engine::core::GameDataElementType::Type::string_type:
			{
				std::string v;
				ar & make_nvp("string_data", v);;
				game_data_element.SetString(std::move(v));
			}
				break;
			case planeta_engine::core::GameDataElementType::Type::array_type:
			{
				std::vector<GameDataElement> v;
				ar & make_nvp("array_data", v);;
				game_data_element.SetArray(std::move(v));
			}
			break;
			case planeta_engine::core::GameDataElementType::Type::complex_type:
			{
				GameDataComplexTypeInstance v;
				ar & make_nvp("complex_data", v);;
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
		void serialize(Archive& ar, planeta_engine::core::GameDataComplexTypeInstance& game_data_complex_type_instance, unsigned int version) {
			split_free(ar, game_data_complex_type_instance, version);
		}
		template<class Archive>
		void save(Archive& ar, const planeta_engine::core::GameDataComplexTypeInstance&  game_data_complex_type_instance, unsigned int) {
			//型IDを保存
			ar & make_nvp("type_id", game_data_complex_type_instance.complex_type().type_id());
			//要素を保存
			ar & make_nvp("elements", game_data_complex_type_instance.elements_);
		}
		template<class Archive>
		void load(Archive& ar, planeta_engine::core::GameDataComplexTypeInstance&  game_data_complex_type_instance, unsigned int) {
			using namespace planeta_engine::core;
			//型IDを読み込み
			std::string type_id;
			ar & make_nvp("type_id", type_id);
			game_data_complex_type_instance.SetType_for_boost_serialize_(GameDataElementType::GetComplexType(type_id));
			//要素を読み込み
			ar & make_nvp("elements", game_data_complex_type_instance.elements_);
		}
	}
}