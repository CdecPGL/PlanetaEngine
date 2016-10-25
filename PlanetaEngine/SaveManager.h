#pragma once

#include <memory>
#include "ISaveManager.h"
#include "NonCopyable.h"

namespace planeta {
	class FileAccessor;
	namespace private_ {
		class SaveManager : public ISaveManager, private util::NonCopyable<SaveManager> {
		public:
			virtual ~SaveManager()override = default;
			//�t�@�C���A�N�Z�T��ݒ�B�������O�ɌĂяo���B
			virtual void SetFileAccessor_(const std::shared_ptr<FileAccessor>& file_accessor) = 0;
			virtual bool Initialize() = 0;
			virtual void Finalize() = 0;
		};
	}
}