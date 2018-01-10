#pragma once
#include "planeta/FileManipulator.hpp"
#include<unordered_map>

class Extracter;

namespace plnt {
	class ArchiveManipulator final : public FileManipulator {
	public:
		ArchiveManipulator();
		~ArchiveManipulator();
		void SetKey(unsigned int); //復号化キーセット
	private:
		bool OpenProc(const std::string& path)override;
		void CloseProc()override;
		std::unique_ptr<Extracter> _extracter;
		unsigned int _key; //復号化キー
		bool ReloadProc()override;
		bool LoadFileProc(const std::string&, File& file)override;
		bool SaveFileProc(const std::string& name, const File& file) override { return false; }
		bool CheckFileExistenceProc(const std::string& path) const override;
		size_t GetFileCountProc() const override;
		bool GetAllFilePathsProc(std::vector<std::string>& path_list) const override;

	};
}
