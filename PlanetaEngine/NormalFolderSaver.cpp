#include <fstream>
#include "NormalFolderSaver.h"
#include "SystemLog.h"
#include "boost/filesystem.hpp"
#include "File.h"
#include "EncrypterBase.h"

namespace planeta_engine {
	namespace file_system {
		bool NormalFileSaver::SaveFile(const std::string& name, const File& file)
		{
			std::ofstream ofs(path() + "\\" + name, std::ios::binary | std::ios::trunc);
			if (!ofs) { 
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "ファイル", name, "をディレクトリ", path(), "に保存できませんでした");
				return false;
			}
			if (is_encrypter_valid()) { //暗号化が有効だったら
				File encd_file;
				if (!encrypter()->Encrypt(file, encd_file)) { //暗号化して
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "暗号化に失敗しました。");
					return false;
				}
				ofs.write(reinterpret_cast<char*>(encd_file.GetTopPointer()), encd_file.GetSize()); //保存
			}
			else {
				ofs.write(reinterpret_cast<const char*>(file.GetTopPointer()), file.GetSize());
			}
			return true;
		}

		bool NormalFileSaver::InitializeCore()
		{
			//パスの存在確認
			boost::filesystem::path rel_path(path());
			if (boost::filesystem::exists(rel_path) && boost::filesystem::is_directory(rel_path)) {
				return true;
			}
			else {
				if (auto_create()) {
					if (boost::filesystem::create_directories(rel_path)) { return true; }
					else { 
						debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "初期化に失敗しました。ディレクトリ", path(), "の作成に失敗しました");
						return false; 
					}
				}
				else {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "初期化に失敗しました。ディレクトリ", path(), "が存在しません。");
					return false;
				}
			}
		}

		void NormalFileSaver::FinalizeCore()
		{

		}
	}
}

