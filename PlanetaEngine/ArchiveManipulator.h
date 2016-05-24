#pragma once
#include "FileManipulatorBase.h"
#include<unordered_map>

class Extracter;

namespace planeta_engine {
	class ArchiveManipulator final : public FileManipulatorBase {
	public:
		explicit ArchiveManipulator(const std::string& path);
		ArchiveManipulator(const std::string& path, unsigned int key);
		~ArchiveManipulator();
		void SetKey(unsigned int); //復号化キーセット
	private:
		bool InitializeCore()override;
		void FinalizeCore()override;
		std::unique_ptr<Extracter> _extracter;
		unsigned int _key; //復号化キー
		bool UpdateFileListCore(std::unordered_set<std::string>& file_list)override;
		bool LoadAllFilesCore(std::vector<std::pair<std::string, std::shared_ptr<File>>>& files)override;
		bool LoadFileCore(const std::string&, File& file)override;
		bool SaveFileCore(const std::string& name, const File& file) override { return false; }
	};
}
