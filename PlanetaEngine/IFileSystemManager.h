#pragma once

#include <memory>
#include <string>

namespace planeta {
	class FileAccessor;
	/*! ファイルシステムマネージャ公開インターフェイス*/
	class IFileSystemManager {
	public:
		virtual ~IFileSystemManager() = 0 {};
		/*! ファイルアクセサを取得する*/
		virtual std::shared_ptr<FileAccessor> GetFileAccessor(const std::string& id)const = 0;
	};
}
