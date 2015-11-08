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
boost::serialize���C�u������p����GameDataElement��GameDataComplexTypeInstance���V���A���C�Y���邽�߂̊֐��Q�B
�ł��邾��boost���C�u�����̈ˑ��������邽�߂ɁA��N���^���̗p�����B
�������V���A���C�Y�̂��߂ɁA�K�v�Ȋ֐��͊e�N���X��friend�w�肵�A�V���A���C�Y�ŗp����֐���ǉ������B
����GameDataComplexTypeInstance��GameDataComplexType�̎Q�Ƃ�ێ����邪�A�V���A���C�Y���ɕύX����K�v�����邽�߃|�C���^�ŕێ�����悤�ɂ��Ă���B
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
			//�^ID��ۑ�
			ar << game_data_element.type_id();
			//�^�̎�ނɉ����ĕۑ�
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
				throw GameDataError("������GameData�ł��B");
				break;
			}
		}
		template<class Archive>
		void load(Archive& ar, planeta_engine::core::GameDataElement& game_data_element, unsigned int) {
			using namespace planeta_engine::core;
			//�^ID��ǂݍ���
			std::string type_id;
			ar >> type_id;
			game_data_element.SetType_for_boost_serialize_(type_id);
			//�^�̎�ނɉ����ĕۓǂݍ���
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
				throw GameDataError("������GameData�ł��B");
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
			//�^ID��ۑ�
			ar << game_data_complex_type_instance.complex_type().type_id();
			//�v�f��ۑ�
			ar << game_data_complex_type_instance.elements_;
		}
		template<class Archive>
		void load(Archive& ar, planeta_engine::core::GameDataComplexTypeInstance&  game_data_complex_type_instance, unsigned int) {
			//�^ID��ǂݍ���
			std::string type_id;
			ar >> type_id;
			game_data_complex_type_instance.SetType_for_boost_serialize_(GameDataElementType::GetComplexType(type_id));
			//�v�f��ǂݍ���
			ar >> game_data_complex_type_instance.elements_;
		}
	}
}