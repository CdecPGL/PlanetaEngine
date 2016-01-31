#include "boost/lexical_cast.hpp"
#include "SaveDataManager.h"
#include "DataContainer.h"
#include "DataContainerSerializeUtility.h"
#include "FileAccessor.h"
#include "SystemLog.h"
#include "CSVResource.h"
#include "MakeResource.h"

namespace {
	constexpr char* CommonSaveDataFileName("common_save_data");
	constexpr char* UserSaveDataFileName("user_save_data");
	constexpr char* SaveDataInformationFileName("save_data_info");
}

namespace planeta_engine {
	namespace core {
		//////////////////////////////////////////////////////////////////////////
		//Impl_
		//////////////////////////////////////////////////////////////////////////
		class SaveDataManager::Impl_ {
		public:
			std::unique_ptr<utility::DataContainer> common_data_;
			std::unique_ptr<utility::DataContainer> current_user_data_;
			std::shared_ptr<file_system::FileAccessor> file_accessor_;
			int current_user_data_idx_ = 0;

			bool LoadSaveDataInformation() {
				//�Z�[�u�f�[�^���̓ǂݍ���
				auto file = file_accessor_->LoadFile(SaveDataInformationFileName);
				if (file == nullptr) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�Z�[�u�f�[�^���t�@�C���̎擾�Ɏ��s���܂����B");
					return false;
				}
				//������

				return true;
			}
			bool LoadCommonDaveData() {
				auto file = file_accessor_->LoadFile(CommonSaveDataFileName);
				if (file == nullptr) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "���ʃZ�[�u�f�[�^�t�@�C���̎擾�Ɏ��s���܂����B");
					return false;
				}
				try {
					common_data_ = std::make_unique<utility::DataContainer>(std::move(utility::DeserializeDataContainer(file)));
				} catch (std::runtime_error&) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "���ʃZ�[�u�f�[�^�̓ǂݍ��݂Ɏ��s���܂����B");
					return false;
				}
				return true;
			}
			bool SaveSaveDataInformation();
			bool SaveCommonSaveData() {
				auto file = utility::SerializeDataContainer(*common_data_);
				if (file == nullptr) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "���ʃZ�[�u�f�[�^�̃t�@�C���쐬�Ɏ��s���܂����B");
					return false;
				}
				if (!file_accessor_->SaveFile(CommonSaveDataFileName, std::move(*file))) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "���ʃZ�[�u�f�[�^�̕ۑ��Ɏ��s���܂����B");
					return false;
				}
				return true;
			}
		};


		//////////////////////////////////////////////////////////////////////////
		//SaveDataManager
		//////////////////////////////////////////////////////////////////////////
		SaveDataManager::SaveDataManager() :impl_(std::make_unique<Impl_>()) {}

		SaveDataManager::~SaveDataManager() = default;

		bool SaveDataManager::Initialize() {
			if (impl_->file_accessor_ == nullptr) {
				debug::SystemLog::instance().LogError("�t�@�C���A�N�Z�T���ݒ肳��Ă��܂���B", __FUNCTION__);
				return false;
			}
			//�Z�[�u�f�[�^���̓ǂݍ���
			if (!impl_->LoadSaveDataInformation()) { return false; }
			//���ʃZ�[�u�f�[�^�̃��[�h
			if (!impl_->LoadCommonDaveData()) { return false; }

			debug::SystemLog::instance().Log(debug::LogLevel::Message, __FUNCTION__, "�Z�[�u�f�[�^�𐳏�ɓǂݍ��݂܂����B");
			return true;
		}

		bool SaveDataManager::Finalize() {
			Save();
			return true;
		}

		void SaveDataManager::SetFileAccessor(const std::shared_ptr<file_system::FileAccessor>& file_accessor) {
			impl_->file_accessor_ = file_accessor;
		}

		void SaveDataManager::Save() {
			//���ʃf�[�^
			impl_->SaveCommonSaveData();
			//���[�U�[�f�[�^

			//�Z�[�u�f�[�^���X�g�X�V
			impl_->SaveSaveDataInformation();
		}

	}
}