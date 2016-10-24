#pragma once

#include <memory>
#include <string>

namespace planeta {
	class FileAccessor;
	/*! �t�@�C���V�X�e���}�l�[�W�����J�C���^�[�t�F�C�X*/
	class IFileSystemManager {
	public:
		virtual ~IFileSystemManager() = 0 {};
		/*! �t�@�C���A�N�Z�T���擾����*/
		virtual std::shared_ptr<FileAccessor> GetFileAccessor(const std::string& id)const = 0;
	};
}
