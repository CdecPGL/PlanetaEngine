#pragma once
#include "LoaderBase.h"
#include<unordered_map>

namespace planeta_engine{
	namespace file_system{

		class NormalFolderLoader :
			public LoaderBase
		{
		public:
			NormalFolderLoader(const std::string&);
			~NormalFolderLoader()override;
			int LoadAllFileToCache()override;
			int DeleteCache()override;
			int UpdateFileList()override;
			std::shared_ptr<File> LoadFile(const std::string&)override;
			unsigned int GetCacheSize()const override;
		private:
			std::unordered_map<std::string, std::pair<std::string, std::shared_ptr<File>>> _files; //ファイルリスト(<ファイル名<ファイルパス,ファイルポインタ>>)
			static int LoadData(const std::shared_ptr<File>&, const std::string&);
			bool _Initialize()override;
			void _Finalize()override;
		};
	}
}
