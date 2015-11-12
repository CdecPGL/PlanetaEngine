#include <cassert>
#include "FileAccessor.h"
#include "FileLoaderBase.h"
#include "FileSaverBase.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace file_system {
		namespace {
			namespace access_mode {
				using type = uint_fast32_t;
				constexpr type None = 0b00;
				constexpr type Read = 0b01;
				constexpr type Write = 0b10;
				constexpr type ReadWrite = 0b11;
			}
			constexpr access_mode::type ConvertAccessModeToUint32(FileAccessor::Mode mode) {
				return mode == planeta_engine::file_system::FileAccessor::ReadOnly ? access_mode::Read :
					mode == planeta_engine::file_system::FileAccessor::WriteOnly ? access_mode::Write :
					mode == planeta_engine::file_system::FileAccessor::ReadWrite ? access_mode::ReadWrite :
					mode == planeta_engine::file_system::FileAccessor::Invalid ? access_mode::None : access_mode::None;
			}
			constexpr FileAccessor::Mode ConvertUint32ToAccessMode(access_mode::type amode_uint32) {
				return amode_uint32 == access_mode::Read ? FileAccessor::Mode::ReadOnly :
					amode_uint32 == access_mode::Write ? FileAccessor::Mode::WriteOnly :
					amode_uint32 == access_mode::ReadWrite ? FileAccessor::Mode::ReadWrite :
					amode_uint32 == access_mode::None ? FileAccessor::Mode::Invalid : FileAccessor::Mode::Invalid;
			}
		}

		void FileAccessor::PushLoader(const std::shared_ptr<FileLoaderBase>& loader)
		{
			if (is_initialized_) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "���[�_�[�̒ǉ��͏������O�ɍs���K�v������܂��B");
				return;
			}
			mode_ = ConvertUint32ToAccessMode(ConvertAccessModeToUint32(mode_) | access_mode::Read);
			loaders_.push_back(loader);
		}

		std::shared_ptr<const File> FileAccessor::LoadFile(const std::string& file_name) const
		{
			if ((ConvertAccessModeToUint32(mode_) | access_mode::Read) != access_mode::Read) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�ǂݍ��ݑ���������܂���B");
				return nullptr;
			}
			auto it = file_caches_.find(file_name);
			if (it == file_caches_.end()) {
				auto file_ptr = LoadProc(file_name);
				if (file_ptr) {
					file_caches_.emplace(file_name, file_ptr);
					return file_ptr;
				}
				else {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�t�@�C����ǂݍ��߂܂���ł����B");
					return nullptr;
				}
			}
			else {
				return it->second;
			}
		}

		bool FileAccessor::LoadAllFilesToCache()
		{
			if ((ConvertAccessModeToUint32(mode_) | access_mode::Read) != access_mode::Read) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�ǂݍ��ݑ���������܂���B");
				return false;
			}
			for (auto& loader : loaders_) {
				auto files = std::move(loader->LoadAllFiles());
				for (auto file : files) {
					file_caches_.emplace(file.first, file.second);
				}
			}
			return true;
		}

		void FileAccessor::SetSaver(const std::shared_ptr<FileSaverBase>& saver)
		{
			if (is_initialized_) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "Saver�̐ݒ�͏������O�ɍs���K�v������܂��B");
				return;
			}
			mode_ = ConvertUint32ToAccessMode(ConvertAccessModeToUint32(mode_) | access_mode::Write);
			saver_ = saver;
		}

		bool FileAccessor::SaveFile(const std::string& file_name, const File& file)
		{
			if ((ConvertAccessModeToUint32(mode_) | access_mode::Write) != access_mode::Write) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�������ݑ���������܂���B");
				return false;
			}
			*file_caches_[file_name] = file;
			return true;
		}

		bool FileAccessor::SaveFile(const std::string& file_name, File&& file)
		{
			if ((ConvertAccessModeToUint32(mode_) | access_mode::Write) != access_mode::Write) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�������ݑ���������܂���B");
				return false;
			}
			*file_caches_[file_name] = std::move(file);
			return true;
		}

		bool FileAccessor::SaveAllFilesFromCache()
		{
			if ((ConvertAccessModeToUint32(mode_) | access_mode::Write) != access_mode::Write) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�������ݑ���������܂���B");
				return false;
			}

			std::vector<std::pair<std::string,const File&>> files;
			for (const auto& fc : file_caches_) {
				files.push_back(std::make_pair(fc.first, *fc.second));
			}
			return saver_->SaveFiles(files);
		}

		std::shared_ptr<File> FileAccessor::LoadProc(const std::string& file_name) const
		{
			assert(loaders_.size() > 0);
			std::shared_ptr<File> file;
			for (auto& ldr : loaders_) {
				if ((file = ldr->LoadFile(file_name)) != nullptr) { break; }
			}
			if (!file) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�t�@�C��", file_name, "���ǂݍ��߂܂���ł����B");
			}
			return file;
		}

		bool FileAccessor::SaveProc(const std::string& file_name, const File& file)
		{
			assert(saver_ != nullptr);
			return saver_->SaveFile(file_name, file);
		}

		void FileAccessor::DeleteCache()
		{
			if ((ConvertAccessModeToUint32(mode_) | access_mode::Write) == access_mode::Write) {
				SaveAllFilesFromCache();
			}
			file_caches_.clear();
		}

		size_t FileAccessor::GetCacheSize() const
		{
			size_t size = 0;
			for (auto& c : file_caches_) { size += c.second->GetSize(); }
			return size;
		}

		bool FileAccessor::Initialize()
		{
			if (is_initialized_) { return true; } //�������ς݂��������΂�
			bool saver_error = false;
			if (saver_) { 
				if (!saver_->Initialize()) {
					debug::SystemLog::instance().LogMessage("FileSaver�̏������Ɏ��s���܂����B", __FUNCTION__);
					saver_error = true;
				}
			}
			bool error_flag = false;
			bool success_flag = false;
			for (auto& loader : loaders_) {
				if (loader->Initialize()) {
					success_flag = true;
				}
				else {
					error_flag = true;
				}
			}
			is_initialized_ = true;
			if ((!success_flag && error_flag) || saver_error ) {
				debug::SystemLog::instance().LogError("�������Ɏ��s���܂����B", __FUNCTION__);
				return true;
			}
			else if (success_flag && error_flag) {
				debug::SystemLog::instance().LogWarning("�ꕔ���[�_�[�̏������Ɏ��s���܂������A�p�����܂��B", __FUNCTION__);
				return true;
			}
			else if(!saver_error && success_flag && !error_flag){
				debug::SystemLog::instance().LogMessage("�������ɐ������܂����B", __FUNCTION__);
				return false;
			}
			else {
				assert(false);
				return false;
			}
		}

		void FileAccessor::Finalize()
		{
			if (!is_initialized_) { return; } //����������Ă��Ȃ��������΂�
			if (saver_) { saver_->Finalize(); }
			for (auto& loader : loaders_) {
				loader->Finalize();
			}
			is_initialized_ = false;
		}

	}
}