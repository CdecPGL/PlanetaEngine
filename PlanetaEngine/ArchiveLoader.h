#pragma once
#include "LoaderBase.h"
#include<unordered_map>

class Extracter;

namespace planeta_engine{
	namespace file_system{
		class ArchiveLoader :
			public LoaderBase
		{
		public:
			ArchiveLoader(const std::string&);
			ArchiveLoader(const std::string&, unsigned int);
			~ArchiveLoader();
			int LoadAllFileToCache()override;
			int DeleteCache()override;
			int UpdateFileList()override;
			void SetKey(unsigned int); //復号化キーセット
			std::shared_ptr<File> LoadFile(const std::string&)override;
			unsigned int GetCacheSize()const override;
		private:
			bool _Initialize()override;
			void _Finalize()override;
			int LoadData(const std::string&, const std::shared_ptr<File>&);
			std::unordered_map<std::string, std::shared_ptr<File>> _files;
			std::unique_ptr<Extracter> _extracter;
			unsigned int _key; //復号化キー
		};
	}
}
