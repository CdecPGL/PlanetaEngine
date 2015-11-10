#pragma once
#include "FileLoaderBase.h"
#include<unordered_map>

namespace planeta_engine{
	namespace file_system{

		class NormalFolderLoader :
			public FileLoaderBase
		{
		public:
			FileLoaderBase::FileLoaderBase;
			~NormalFolderLoader()override;
			bool LoadAllFileToCache()override;
			bool DeleteCache()override;
			bool UpdateFileList()override;
			std::shared_ptr<File> LoadFile(const std::string&)override;
			size_t GetCacheSize()const override;
		private:
			std::unordered_map<std::string, std::pair<std::string, std::shared_ptr<File>>> files_; //ファイルリスト(<ファイル名<ファイルパス,ファイルポインタ>>)
			bool LoadFile(const std::shared_ptr<File>&, const std::string&);
			static int LoadDataCore(const std::shared_ptr<File>&, const std::string&);
			bool _Initialize()override;
			void _Finalize()override;
		};
	}
}
