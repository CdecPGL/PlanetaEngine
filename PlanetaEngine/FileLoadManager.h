#pragma once

#include<string>
#include<vector>
#include"LoaderBase.h"
#include "PointerSingletonTemplate.h"

namespace planeta_engine{
	namespace file_system{
		class FileLoadManager : public utility::PointerSingletonTemplate<FileLoadManager>
		{
			friend utility::PointerSingletonTemplate<FileLoadManager>;
		public:
			bool Initialize()override;
			bool Finalize()override;
			/*ローダーをプッシュ。初期化前に呼び出す必要がある。末尾のローダーから読み込みが行われる*/
			void PushLoader(const std::shared_ptr<LoaderBase>&);
			std::shared_ptr<File> LoadFile(const std::string&);
			int LoadAllFileToCache();
			int DeleteCache();
			int UpdateFileList();
			unsigned int GetCacheSize()const;
		private:
			FileLoadManager();
			FileLoadManager(const FileLoadManager&) = delete;
			~FileLoadManager();
			std::vector<std::shared_ptr<LoaderBase>> _loaders;
		};
	}
}