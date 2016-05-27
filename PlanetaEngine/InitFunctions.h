#pragma once

#include <functional>
#include <tuple>
#include <memory>

namespace planeta_engine {
	class FileAccessor;
	namespace core {
		struct ProgramDefinitionData;
		namespace init_funcs {
			//PlanetaEngine��Init�֐����ŌĂ΂��B�߂�l�ɏ������̐��ۂƁA�I�������֐���Ԃ��B�G���[���b�Z�[�W�͂����̊֐����ŏo�͂���B
			std::tuple<bool, std::function<void()>> InitializeLogSystem();
			std::tuple<bool,std::function<void()>> InitializeDxLib();
			std::tuple<bool, std::function<void()>> InitializeResourceSystem(const std::shared_ptr<FileAccessor> res_file_sccessor);
			//���̑��������⏕�֐�
			bool LoadEngineConfig(const std::shared_ptr<FileAccessor> sys_dir_sccessor);
			enum class FileAccessorKind{Resource,SaveData,System,Config};
			std::shared_ptr<FileAccessor> CreateFileAccessor(FileAccessorKind kind);
			bool LoadProgramDefinition(const std::shared_ptr<FileAccessor> sys_dir_accessor,ProgramDefinitionData* out);
		}
	}
}