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
			std::shared_ptr<FileAccessor> file_accessor_;
			int current_user_data_idx_ = -1; //-1�œǂݍ���ł��Ȃ�

			struct SaveDataInfo {
				int user_save_data_count = 0;
				int user_save_data_param_count = 0;
				std::vector<std::pair<int,UserDataHeader>> user_data_info; //<�ԍ�,���[�U�[�f�[�^>
			};

			SaveDataInfo save_data_info_;

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
			bool LoadCommonSaveData() {
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
			bool LoadUserSaveData(int idx) {
				if (idx < 0 || save_data_info_.user_save_data_count <= idx) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�͈͊O�̃��[�U�[�Z�[�u�f�[�^�ԍ����w�肳��܂����B(", idx, ")");
					return false;
				}
				std::string file_name = std::string(UserSaveDataFileName) + boost::lexical_cast<std::string>(save_data_info_.user_data_info[idx].first);
				auto file = file_accessor_->LoadFile(file_name);
				if (file == nullptr) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "���[�U�[�Z�[�u�f�[�^", idx, "�t�@�C���̎擾�Ɏ��s���܂����B");
					return false;
				}
				try {
					current_user_data_ = std::make_unique<utility::DataContainer>(std::move(utility::DeserializeDataContainer(file)));
				} catch (std::runtime_error&) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "���[�U�[�Z�[�u�f�[�^", idx, "�̓ǂݍ��݂Ɏ��s���܂����B");
					return false;
				}
				return true;
			}
			bool SaveSaveDataInformation() {
				if (current_user_data_ < 0 || save_data_info_.user_save_data_count <= current_user_data_idx_) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�͈͊O�̃��[�U�[�Z�[�u�f�[�^�ԍ����w�肳��܂����B(", current_user_data_idx_, ")");
					return false;
				}
				assert(current_user_data_ != nullptr);
				auto file = utility::SerializeDataContainer(*current_user_data_);
				if (file == nullptr) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "���[�U�[�Z�[�u�f�[�^", current_user_data_idx_, "�̃t�@�C���쐬�Ɏ��s���܂����B");
					return false;
				}
				if (!file_accessor_->SaveFile(std::string(UserSaveDataFileName) + boost::lexical_cast<std::string>(current_user_data_idx_), std::move(*file))) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "���[�U�[�Z�[�u�f�[�^", current_user_data_idx_, "�̕ۑ��Ɏ��s���܂����B");
					return false;
				}
				return true;
			}
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
			bool SaveCurrentUserSaveData() {
				

			}
		};


		//////////////////////////////////////////////////////////////////////////
		//SaveDataManager
		//////////////////////////////////////////////////////////////////////////
		SaveDataManager::SaveDataManager() :impl_(std::make_unique<Impl_>()) {}

		SaveDataManager::~SaveDataManager() = default;

		bool SaveDataManager::Initialize() {
			assert(impl_->file_accessor_ != nullptr);
			//�Z�[�u�f�[�^���̓ǂݍ���
			//if (!impl_->LoadSaveDataInformation()) { return false; }
			//���ʃZ�[�u�f�[�^�̃��[�h
			//if (!impl_->LoadCommonSaveData()) { return false; }

			//debug::SystemLog::instance().Log(debug::LogLevel::Message, __FUNCTION__, "�Z�[�u�f�[�^�𐳏�ɓǂݍ��݂܂����B");
			PE_LOG_WARNING("SaveDataManager�͖������ł��B");
			return true;
		}

		void SaveDataManager::Finalize() {
			//Save();
		}

		void SaveDataManager::SetFileAccessor_(const std::shared_ptr<FileAccessor>& file_accessor) {
			impl_->file_accessor_ = file_accessor;
		}

		void SaveDataManager::Save() {
			//���ʃf�[�^
			impl_->SaveCommonSaveData();
			//���[�U�[�f�[�^

			//�Z�[�u�f�[�^���X�g�X�V
			impl_->SaveSaveDataInformation();
		}

		bool SaveDataManager::LoadUserData(int idx) {
			return impl_->LoadUserSaveData(idx);
		}

		int SaveDataManager::GetUserDataConut() const {
			return impl_->save_data_info_.user_save_data_count;
		}

		int SaveDataManager::GetUserDataHeaderParamCount() const {
			return impl_->save_data_info_.user_save_data_param_count;
		}

		const utility::DataContainer& SaveDataManager::GetCommonData() const {
			assert(impl_->common_data_ != nullptr);
			return  *impl_->common_data_;
		}

		utility::DataContainer& SaveDataManager::GetCommonData() {
			assert(impl_->common_data_ != nullptr);
			return  *impl_->common_data_;
		}

		boost::optional<const utility::DataContainer&> SaveDataManager::GetCurrentData() const {
			if (impl_->current_user_data_idx_ >= 0) {
				assert(impl_->current_user_data_ != nullptr);
				return boost::optional<const utility::DataContainer&>(*impl_->common_data_);
			} else {
				return boost::optional<const utility::DataContainer&>();
			}
		}

		boost::optional<utility::DataContainer&> SaveDataManager::GetCurrentData() {
			if (impl_->current_user_data_idx_ >= 0) {
				assert(impl_->current_user_data_ != nullptr);
				return boost::optional<utility::DataContainer&>(*impl_->common_data_);
			} else {
				return boost::optional<utility::DataContainer&>();
			}
		}

	}
}