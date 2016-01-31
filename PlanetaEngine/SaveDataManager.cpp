#include "SaveDataManager.h"
#include "DataContainer.h"
#include "FileAccessor.h"

namespace planeta_engine {
	namespace core {

		SaveDataManager::~SaveDataManager() = default;

		bool SaveDataManager::Initialize() {
			if (file_accessor_ == nullptr) {

				return false;
			}
			//���ʃZ�[�u�f�[�^�̃��[�h

			//���[�U�[�Z�[�u�f�[�^���X�g�̓ǂݍ���

			return true;
		}

		bool SaveDataManager::Finalize() {
			file_accessor_->SaveAllFilesFromCache();
			return true;
		}

		void SaveDataManager::SetFileAccessor(const std::shared_ptr<file_system::FileAccessor>& file_accessor) {
			file_accessor_ = file_accessor;
		}

	}
}