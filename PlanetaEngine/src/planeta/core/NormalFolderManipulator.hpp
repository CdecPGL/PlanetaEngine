#pragma once
#include "FileManipulator.hpp"
#include<unordered_set>

namespace plnt {
	class NormalFolderManipulator final : public FileManipulator {
	public:
		using FileManipulator::FileManipulator;
		~NormalFolderManipulator() override;

	private:
		bool LoadFileByPath(File &, const std::string &);
		static int LoadDataCore(File &, const std::string &);
		bool OpenProc(const std::string &path) override;
		void CloseProc() override;
		bool LoadFileProc(const std::string &, File &file) override;
		bool SaveFileProc(const std::string &name, const File &file) override;
		size_t GetFileCountProc() const override;
		bool CheckFileExistenceProc(const std::string &path) const override;
		bool GetAllFilePathsProc(std::vector<std::string> &path_list) const override;
	};
}
