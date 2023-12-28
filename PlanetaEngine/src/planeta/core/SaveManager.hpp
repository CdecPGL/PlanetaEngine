#pragma once

#include <memory>
#include "ISaveManager.hpp"
#include "SubSystemManager.hpp"

namespace plnt {
	class file_manipulator;

	namespace private_ {
		class SaveManager : public ISaveManager, public SubSystemManager {
		public:
			virtual ~SaveManager() override = default;
			//ファイルアクセサを設定。初期化前に呼び出す。
			virtual void SetFileManipurator_(const std::shared_ptr<file_manipulator> &file_accessor) = 0;
			virtual bool Initialize() = 0;
			virtual void Finalize() = 0;
		};
	}
}
