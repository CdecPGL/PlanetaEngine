#pragma once

#include "IFileSystemManager.h"
#include "NonCopyable.h"
#include "FileAccessMode.h"

namespace planeta {
	class FileManipulatorBase;
	namespace private_ {
		/*�t�@�C���V�X�e���}�l�[�W��*/
		class FileSystemManager : public IFileSystemManager, private util::NonCopyable<FileSystemManager> {
		public:
			virtual ~FileSystemManager()override = default;
			/*������*/
			virtual bool Initialize() = 0;
			/*�I������*/
			virtual void Finalize() = 0;
			virtual std::shared_ptr<FileAccessor> CreateFileAccessor(const std::string& id, const std::shared_ptr<FileManipulatorBase>& manipulator, AccessMode mode) = 0;
			virtual bool DisposeFileAccessor(const std::string& id) = 0;
			virtual bool DeleteCache() = 0;
			virtual size_t GetCacheSize()const = 0;
		};
	}
}
