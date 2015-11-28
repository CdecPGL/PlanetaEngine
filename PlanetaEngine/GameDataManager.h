#pragma once

#include <unordered_map>
#include <memory>
#include "GameDataElement.h"
#include "PointerSingletonTemplate.h"

namespace planeta_engine {
	namespace file_system {
		class FileAccessor;
	}
	namespace core {
		class GameDataManager final : public utility::PointerSingletonTemplate<GameDataManager>{
			friend class utility::PointerSingletonTemplate<GameDataManager>;
		public:
			bool Initialize()override;
			bool Finalize()override;
			/**
			* @brief �f�[�^���擾����
			* @param �f�[�^ID
			* @return �f�[�^�̃X�}�|�B���݂��Ȃ��Ƃ���nullptr
			*/
			std::shared_ptr<GameDataElement> GetElement(const std::string& data_id);
			/**
			* @brief �f�[�^��ݒ肷��
			* @param �f�[�^��ID�ƃf�[�^�̉E�Ӓl�Q�ƁB�����ɓn�����f�[�^�͋�ɂȂ�̂Œ��ӁB�߂�l�ɃX�}�|��Ԃ��̂ň����p�}�험�p����ꍇ�͂�����g���B
			* @brief �����ɓn�����f�[�^�̃X�}�|�B
			*/
			std::shared_ptr<GameDataElement> SetElement(const std::string& data_id,GameDataElement&& data);
			/**
			* @brief �f�[�^��ۑ�����
			* @param �f�[�^ID�Ɠ����t�@�C�����ŕۑ�����B�V�X�e���̃Z�[�u�f�[�^�f�B���N�g���ɕۑ�����B
			* @return �ۑ��ɐ����������ۂ�
			*/
			bool SaveGameData(const std::string& data_id);
			/**
			* @brief �f�[�^��ǂݍ���
			* @param �V�X�e���̃Z�[�u�f�[�^�f�B���N�g���̎w�薼�t�@�C������f�[�^��ǂݍ��ށB�f�[�^ID�̓t�@�C�����Ɠ������̂ɂȂ�B
			* @return �ǂݍ��݂ɐ����������ۂ�
			*/
			bool LoadGameData(const std::string& file_name);



			static void ResistComplexType(const std::string& type_id,std::unordered_map<std::string,std::string>&& element_types);
		private:
			GameDataManager() = default;
			~GameDataManager() = default;
			std::shared_ptr<file_system::FileAccessor> file_accessor_;
			std::unordered_map<std::string, std::shared_ptr<GameDataElement>> datas_;
			static bool CheckComplexTypeElementTypeError();
		};
	}
}