#pragma once

#include <memory>
#include "ISaveManager.h"
#include "SubSystemManager.h"

namespace planeta {
	class FileAccessor;
	namespace private_ {
		class SaveManager : public ISaveManager, public SubSystemManager {
		public:
			virtual ~SaveManager()override = default;
			//ファイルアクセサを設定。初期化前に呼び出す。
			virtual void SetFileAccessor_(const std::shared_ptr<FileAccessor>& file_accessor) = 0;
			virtual bool Initialize() = 0;
			virtual void Finalize() = 0;
		};
	}
}