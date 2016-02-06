#pragma once

#include <memory>
#include <vector>
#include "PointerSingletonTemplate.h"
#include "PEDateTime.h"
#include "boost/optional.hpp"

namespace planeta_engine {
	namespace utility {
		class DataContainer;
	}
	namespace file_system {
		class FileAccessor;
	}
	namespace core {
		class SaveDataManager final: public utility::PointerSingletonTemplate<SaveDataManager>{
			friend class utility::PointerSingletonTemplate<SaveDataManager>;
		public:
			void SetFileAccessor(const std::shared_ptr<file_system::FileAccessor>& file_accessor);
			bool Initialize()override;
			bool Finalize()override;

			struct UserDataHeader {
				utility::DateTime update_time;
				std::vector<std::string> header;
			};
			/*�ۑ�*/
			void Save();
			/*���[�U�[�f�[�^��ǂݍ���*/
			bool LoadUserData(int idx);
			/*���[�U�[�f�[�^�̐����擾*/
			int GetUserDataConut()const;
			/*���[�U�[�f�[�^�̃w�b�_��񐔂��擾*/
			int GetUserDataHeaderParamCount()const;
			/*���[�U�[�f�[�^�̃w�b�_�����擾*/
			const UserDataHeader& GetUserDataHeaderParam(int idx)const;
			/*���ʃf�[�^���擾*/
			const utility::DataContainer& GetCommonData()const;
			/*���ʃf�[�^���擾*/
			utility::DataContainer& GetCommonData();
			/*���[�U�[�f�[�^���擾*/
			boost::optional<const utility::DataContainer&> GetCurrentData()const;
			/*���[�U�[�f�[�^���擾*/
			boost::optional<utility::DataContainer&> GetCurrentData();
			/*���[�U�[�f�[�^��V�K�쐬(�C���f�b�N�X��Ԃ�)*/
			int CreateUserData();
			/*���[�U�[�f�[�^�𕡐�(�C���f�b�N�X��Ԃ�)*/
			int DuplicateUserData();
			/*���[�U�[�f�[�^���폜*/
			bool DeleteUserData(int idx);
		private:
			SaveDataManager();
			~SaveDataManager();
			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	}
}