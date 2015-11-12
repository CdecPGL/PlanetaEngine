#include "FileSaverBase.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace file_system {
		bool FileSaverBase::SaveFiles(const std::vector<std::pair<std::string, const File&>>& files)
		{
			bool err;
			for (const auto& f : files) {
				if (!SaveFile(f.first, f.second)) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�t�@�C��", f.first, "�̕ۑ��Ɏ��s���܂����B(�p�X ", path(), ",�^�C�v ", GetType().name(), ")");
					err = true;
				}
			}
			return !err;
		}
	}
}