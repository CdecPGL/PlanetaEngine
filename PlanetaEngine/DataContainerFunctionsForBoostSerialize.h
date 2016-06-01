#pragma once

#include "boost/serialization/serialization.hpp"
#include "boost/serialization/string.hpp"
#include "boost/serialization/unordered_map.hpp"
#include "boost/serialization/level.hpp"
#include "boost/serialization/nvp.hpp"
#include "boost/serialization/split_free.hpp"

#include "DataContainerError.h"
#include "DataContainer.h"
#include "DataContainerImpl.h"

/*
boost::serialize���C�u������p����GameData���V���A���C�Y���邽�߂̊֐��Q�B
�ł��邾��boost���C�u�����̈ˑ��������邽�߂ɁA��N���^���̗p�����B
�������V���A���C�Y�̂��߂ɁA�K�v�Ȋ֐��͊e�N���X��friend�w�肵�A�V���A���C�Y�ŗp����֐���ǉ������B
*/
namespace boost {
	namespace serialization {
		/*GameDataElement*/
		template<class Archive>
		void serialize(Archive& ar, planeta::util::DataContainer& data_container, unsigned int version) {
			split_free(ar, data_container, version);
		}
		template<class Archive>
		void save(Archive& ar, const planeta::util::DataContainer& data_container, unsigned int) {
			using namespace planeta::util;
			auto& dci = *data_container.impl_;
			//�����v�f��ۑ�
			ar & make_nvp("integer_elements", dci.integer_elements_);
			//�_���l�v�f��ۑ�
			ar & make_nvp("boolean_elements", dci.boolean_elements_);
			//������v�f��ۑ�
			ar & make_nvp("string_elements", dci.string_elements_);
			//�����v�f��ۑ�
			ar & make_nvp("number_elements", dci.number_elements_);
		}
		template<class Archive>
		void load(Archive& ar, planeta::util::DataContainer& data_container, unsigned int) {
			using namespace planeta::util;
			auto& dci = *data_container.impl_;
			//�����v�f��ǂݍ���
			ar & make_nvp("integer_elements", dci.integer_elements_);
			//�_���l�v�f��ǂݍ���
			ar & make_nvp("boolean_elements", dci.boolean_elements_);
			//������v�f��ǂݍ���
			ar & make_nvp("string_elements", dci.string_elements_);
			//�����v�f��ǂݍ���
			ar & make_nvp("number_elements", dci.number_elements_);
			//�v�f�^�}�b�v���\�z
			dci.ConstructElementTypeMap();
		}
	}
}