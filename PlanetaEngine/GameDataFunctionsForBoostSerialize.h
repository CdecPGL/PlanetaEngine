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
boost::serialize���C�u������p����GameDataElement��GameDataComplexTypeInstance���V���A���C�Y���邽�߂̊֐��Q�B
�ł��邾��boost���C�u�����̈ˑ��������邽�߂ɁA��N���^���̗p�����B
�������V���A���C�Y�̂��߂ɁA�K�v�Ȋ֐��͊e�N���X��friend�w�肵�A�V���A���C�Y�ŗp����֐���ǉ������B
����GameDataComplexTypeInstance��GameDataComplexType�̎Q�Ƃ�ێ����邪�A�V���A���C�Y���ɕύX����K�v�����邽�߃|�C���^�ŕێ�����悤�ɂ��Ă���B
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
			//�^ID��ۑ�
			ar & make_nvp("type_id", game_data_element.type_id());
			//�^�̎�ނɉ����ĕۑ�
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
				throw GameDataError("������GameData�ł��B");
				break;
			}
		}
		template<class Archive>
		void load(Archive& ar, planeta_engine::core::GameDataElement& game_data_element, unsigned int) {
			using namespace planeta_engine::core;
			//�^ID��ǂݍ���
			std::string type_id;
			ar & make_nvp("type_id", type_id);
			game_data_element.SetType_for_boost_serialize_(type_id);
			//�^�̎�ނɉ����ĕۓǂݍ���
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
				throw GameDataError("������GameData�ł��B");
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
			//�^ID��ۑ�
			ar & make_nvp("type_id", game_data_complex_type_instance.complex_type().type_id());
			//�v�f��ۑ�
			ar & make_nvp("elements", game_data_complex_type_instance.elements_);
		}
		template<class Archive>
		void load(Archive& ar, planeta_engine::core::GameDataComplexTypeInstance&  game_data_complex_type_instance, unsigned int) {
			using namespace planeta_engine::core;
			//�^ID��ǂݍ���
			std::string type_id;
			ar & make_nvp("type_id", type_id);
			game_data_complex_type_instance.SetType_for_boost_serialize_(GameDataElementType::GetComplexType(type_id));
			//�v�f��ǂݍ���
			ar & make_nvp("elements", game_data_complex_type_instance.elements_);
		}
	}
}