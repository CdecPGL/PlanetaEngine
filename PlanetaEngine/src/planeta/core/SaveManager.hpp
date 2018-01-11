#pragma once

#include <memory>
#include "planeta/core/ISaveManager.hpp"
#include "SubSystemManager.hpp"

namespace plnt {
	class FileManipulator;
	namespace private_ {
		class SaveManager : public ISaveManager, public SubSystemManager {
		public:
			virtual ~SaveManager()override = default;
			//ファイルアクセサを設定。初期化前に呼び出す。
			virtual void SetFileManipurator_(const std::shared_ptr<FileManipulator>& file_accessor) = 0;
			virtual bool Initialize() = 0;
			virtual void Finalize() = 0;
		};
	}
}