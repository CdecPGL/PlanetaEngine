#pragma once

#include <memory>
#include <vector>
#include "SingletonTemplate.h"
#include "PEDateTime.h"
#include "boost/optional.hpp"

namespace planeta {
	namespace util {
		class DataContainer;
	}
	class FileAccessor;
	namespace core {
		class SaveDataManager final: public util::SingletonTemplate<SaveDataManager>{
			friend class util::SingletonTemplate<SaveDataManager>;
		public:
			//�t�@�C���A�N�Z�T��ݒ�B�������O�ɌĂяo���B
			void SetFileAccessor_(const std::shared_ptr<FileAccessor>& file_accessor);
			bool Initialize()override;
			void Finalize()override;

			struct UserDataHeader {
				util::DateTime update_time;
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
			const util::DataContainer& GetCommonData()const;
			/*���ʃf�[�^���擾*/
			util::DataContainer& GetCommonData();
			/*���[�U�[�f�[�^���擾*/
			boost::optional<const util::DataContainer&> GetCurrentData()const;
			/*���[�U�[�f�[�^���擾*/
			boost::optional<util::DataContainer&> GetCurrentData();
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