#pragma once
#include "FileManipulatorBase.h"
#include<unordered_set>

namespace planeta {
	class NormalFolderManipulator final : public FileManipulatorBase {
	public:
		using FileManipulatorBase::FileManipulatorBase;
		~NormalFolderManipulator()override;
	private:
		bool LoadFileByPath(File&, const std::string&);
		static int LoadDataCore(File&, const std::string&);
		bool InitializeProc()override;
		void FinalizeProc()override;
		bool LoadFileProc(const std::string&, File& file)override;
		bool SaveFileProc(const std::string& name, const File& file) override;
		size_t GetFileCountProc()const override;
		bool CheckFileExistenceProc(const std::string& path)const override;
		bool GetAllFilePathsProc(std::vector<std::string>& path_list) const override;

	};
}
