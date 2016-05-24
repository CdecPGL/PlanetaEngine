#pragma once
#include "FileManipulatorBase.h"
#include<unordered_map>

namespace planeta_engine {
	class NormalFolderManipulator final : public FileManipulatorBase {
	public:
		using FileManipulatorBase::FileManipulatorBase;
		~NormalFolderManipulator()override;
	private:
		bool LoadFileByPath(File&, const std::string&);
		static int LoadDataCore(File&, const std::string&);
		bool InitializeCore()override;
		void FinalizeCore()override;
		bool UpdateFileListCore(std::unordered_set<std::string>& file_list)override;
		bool LoadFileCore(const std::string&, File& file)override;
		bool LoadAllFilesCore(std::vector<std::pair<std::string, std::shared_ptr<File>>>& files)override;
		bool SaveFileCore(const std::string& name, const File& file) override;

		std::unordered_map<std::string, std::string> file_name_path_map_;
	};
}
