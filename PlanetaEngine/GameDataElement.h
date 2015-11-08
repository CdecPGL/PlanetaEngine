#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include "GameDataComplexTypeInstance.h"
#include "GameDataError.h"

namespace planeta_engine {
	namespace core {
		class GameDataElement;
	}
}
namespace boost {
	namespace serialization {
		template<class Archive>
		void load(Archive& ar, planeta_engine::core::GameDataElement& game_data_element, unsigned int);
	}
}
namespace planeta_engine{
	namespace core {
		class GameDataElement final{
		public:
			/*�^ID����G�������g���쐬*/
			static GameDataElement CreateByTypeID(const std::string& type_id);
			/*�R�s�[�R���X�g���N�^*/
			GameDataElement(const GameDataElement& element)noexcept;
			/*���[�u�R���X�g���N�^*/
			GameDataElement(GameDataElement&& element)noexcept;
			/*�l�ݒ�p�R���X�g���N�^�Q(�ÖٓI�ϊ���L���ɂ��邽��explicit�͂��Ȃ�)*/
			GameDataElement(int32_t v)noexcept;
			GameDataElement(int64_t v)noexcept;
			GameDataElement(bool v)noexcept;
			GameDataElement(double v)noexcept;
			GameDataElement(const std::string& v)noexcept;
			GameDataElement(std::string&& v)noexcept;
			GameDataElement(const std::vector<GameDataElement>& v)noexcept;
			GameDataElement(std::vector<GameDataElement>&& v)noexcept;
			GameDataElement(const GameDataComplexTypeInstance& v)noexcept;
			GameDataElement(GameDataComplexTypeInstance&& v)noexcept;
			/*������Z�q*/
			GameDataElement& operator=(const GameDataElement& element)noexcept;
			/*���[�u���Z�q*/
			GameDataElement& operator=(GameDataElement&& element)noexcept;
			/*�f�X�g���N�^*/
			~GameDataElement()noexcept;
			//�^���擾
			const std::string type_id()const noexcept;
			//�l���擾
			const int32_t GetInt32()const;
			const int64_t GetInt64()const;
			const bool GetBool()const;
			const double GetDouble()const;
			const std::string& GetString()const;
			const std::vector<GameDataElement>& GetArray()const;
			std::vector<GameDataElement>& GetArrayRef();
			const GameDataComplexTypeInstance& GetComplexType()const;
			GameDataComplexTypeInstance& GetComplexTypeRef();
			//�l��ݒ�
			void SetInt32(int32_t v);
			void SetInt64(int64_t v);
			void SetBool(bool v);
			void SetDouble(double v);
			void SetString(const std::string& v);
			void SetString(std::string&& v);
			void SetArray(const std::vector<GameDataElement>& v);
			void SetArray(std::vector<GameDataElement>&& v);
			void SetComplexType(const GameDataComplexTypeInstance& v);
			void SetComplexType(GameDataComplexTypeInstance&& v);
		private:
			/*�ʏ�̌^�ݒ�R���X�g���N�^(������ݒ�R���X�g���N�^�Ƌ�ʂ��邽�߂̃_�~�[�Ƃ���int�^��������Ɏ��)*/
			GameDataElement(const std::string& type_id, int)noexcept;
			class Impl_;
			std::unique_ptr<Impl_> impl_;
			//boost�V���A���C�Y�̂��߂̃t�����h�w��
			template<class Archive>
			friend void boost::serialization::load<Archive>(Archive& ar, planeta_engine::core::GameDataElement& game_data_element, unsigned int);
			//boost�V���A���C�Y�p
			void SetType_for_boost_serialize_(const std::string& type_id);
		};
	}
}
