#include "ArchiveManipulator.h"
#include"Extracter.h"
#include "Archiver.h"
#include"boost/filesystem/path.hpp"
#include "SystemLog.h"

namespace planeta_engine {
	ArchiveManipulator::ArchiveManipulator(const std::string& path) :FileManipulatorBase(path, false), _extracter(std::make_unique<Extracter>()), _key(0) {}

	ArchiveManipulator::ArchiveManipulator(const std::string& path, unsigned int k) : FileManipulatorBase(path, false), _extracter(std::make_unique<Extracter>()), _key(k) {}

	ArchiveManipulator::~ArchiveManipulator() {
		_extracter->CloseArchiveFile();
	}

	bool ArchiveManipulator::InitializeCore() {
		if (_extracter->SetEXOREncryptionKey((uint16_t)_key)) {
			debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�������Ɏ��s���܂����B�������L�[�̐ݒ�Ɏ��s���܂����B(�p�X ", path(), ")");
			return false;
		}
		int res = _extracter->OpenAchiveFile(path());
		if (res < 0) {
			debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�������Ɏ��s���܂����B�A�[�J�C�u���J���܂���ł����B(���� ", res, ",�p�X ", path(), ")");
			return false;
		}
		return true;
	}

	void ArchiveManipulator::FinalizeCore() {

	}

	bool ArchiveManipulator::LoadAllFilesCore(std::vector<std::pair<std::string, std::shared_ptr<File>>>& files) {
		bool err = false;
		for (const auto& f : file_list()) {
			auto file = std::make_shared<File>();
			if (!LoadFileCore(f, *file)) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�t�@�C��", f, "�̓ǂݍ��݂Ɏ��s���܂����B");
				err = true;
			} else {
				files.push_back(std::make_pair(f, std::move(file)));
			}
		}
		return !err;
	}

	bool ArchiveManipulator::UpdateFileListCore(std::unordered_set<std::string>& file_list) {
		_extracter->CloseArchiveFile();
		int res = _extracter->OpenAchiveFile(path());
		if (res < 0) {
			debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�A�[�J�C�u�t�@�C���̃I�[�v���Ɏ��s���܂����B(���� ", res, ",�p�X ", path(), ")");
			return false;
		}
		//�t�@�C�����X�g�擾
		std::vector<std::string> f_list = std::move(_extracter->GetFileList());
		for (auto& fn : f_list) {
			boost::filesystem::path p(fn);
			file_list.emplace(fn);
		}
		return true;
	}

	bool ArchiveManipulator::LoadFileCore(const std::string& fn, File& file) {
		auto data = _extracter->GetFile(fn);
		if (data.first == nullptr) {
			return false;
		}
		file.SetData(reinterpret_cast<unsigned char*>(data.first), data.second);
		return true;
	}

	void ArchiveManipulator::SetKey(unsigned int k) { _key = k; }
}
