#pragma once

#include <functional>
#include <tuple>
#include <memory>

namespace planeta_engine {
	class FileAccessor;
	namespace core {
		struct ProgramDefinitionData;
		namespace init_funcs {
			//PlanetaEngineのInit関数内で呼ばれる。戻り値に初期化の成否と、終了処理関数を返す。エラーメッセージはこれらの関数内で出力する。
			std::tuple<bool, std::function<void()>> InitializeLogSystem();
			std::tuple<bool,std::function<void()>> InitializeDxLib();
			std::tuple<bool, std::function<void()>> InitializeResourceSystem(const std::shared_ptr<FileAccessor> res_file_sccessor);
			//その他初期化補助関数
			bool LoadEngineConfig(const std::shared_ptr<FileAccessor> sys_dir_sccessor);
			enum class FileAccessorKind{Resource,SaveData,System,Config};
			std::shared_ptr<FileAccessor> CreateFileAccessor(FileAccessorKind kind);
			bool LoadProgramDefinition(const std::shared_ptr<FileAccessor> sys_dir_accessor,ProgramDefinitionData* out);
		}
	}
}