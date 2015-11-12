#pragma once
#include "FileLoaderBase.h"
#include<unordered_map>

class Extracter;

namespace planeta_engine{
	namespace file_system{
		class ArchiveLoader : public FileLoaderBase
		{
		public:
			explicit ArchiveLoader(const std::string&);
			ArchiveLoader(const std::string&, unsigned int);
			~ArchiveLoader();
			void SetKey(unsigned int); //復号化キーセット
		private:
			bool InitializeCore()override;
			void FinalizeCore()override;
			std::unique_ptr<Extracter> _extracter;
			unsigned int _key; //復号化キー
			bool UpdateFileListCore(std::unordered_set<std::string>& file_list)override;
			bool LoadAllFilesCore(std::vector<std::pair<std::string, std::shared_ptr<File>>>& files)override;
			bool LoadFileCore(const std::string&,File& file)override;
		};
	}
}
