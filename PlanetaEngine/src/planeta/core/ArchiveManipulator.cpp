#include "planeta/core/ArchiveManipulator.hpp"
#include"Extracter.h"
#include "Archiver.h"
#include"boost/filesystem/path.hpp"
#include "planeta/core/LogUtility.hpp"

namespace plnt {
	ArchiveManipulator::ArchiveManipulator() :FileManipulator(), _extracter(std::make_unique<Extracter>()), _key(0) {}

	ArchiveManipulator::~ArchiveManipulator() {
		_extracter->CloseArchiveFile();
	}

	bool ArchiveManipulator::OpenProc(const std::string& path) {
		/*if (_extracter->SetEXOREncryptionKey((uint16_t)_key)) {
			PE_LOG_ERROR("初期化に失敗しました。復号化キーの設定に失敗しました。(パス ", path(), ")");
			return false;
		}
		int res = _extracter->OpenAchiveFile(root_path());
		if (res < 0) {
			PE_LOG_ERROR("初期化に失敗しました。アーカイブが開けませんでした。(原因 ", res, ",パス ", path(), ")");
			return false;
		}
		return true;*/
		return false;
	}

	void ArchiveManipulator::CloseProc() {}

	bool ArchiveManipulator::ReloadProc() {
		//_extracter->CloseArchiveFile();
		//int res = _extracter->OpenAchiveFile(root_path());
		//if (res < 0) {
		//	PE_LOG_ERROR("アーカイブファイルのオープンに失敗しました。(原因 ", res, ",パス ", path(), ")");
		//	return false;
		//}
		////ファイルリスト取得
		//std::vector<std::string> f_list = std::move(_extracter->GetFileList());
		//for (auto& fn : f_list) {
		//	boost::filesystem::path p(fn);
		//	file_list.emplace(fn);
		//}
		//return true;
		return false;
	}

	bool ArchiveManipulator::LoadFileProc(const std::string& fn, File& file) {
		/*auto data = _extracter->GetFile(fn);
		if (data.first == nullptr) {
			return false;
		}
		file.MoveData(reinterpret_cast<unsigned char*>(data.first), data.second);
		return true;*/
		return false;
	}

	void ArchiveManipulator::SetKey(unsigned int k) { _key = k; }

	bool ArchiveManipulator::CheckFileExistenceProc(const std::string& path) const {
		return false;
	}

	size_t ArchiveManipulator::GetFileCountProc() const {
		return false;
	}

	bool ArchiveManipulator::GetAllFilePathsProc(std::vector<std::string>& path_list) const {
		return false;
	}

}
