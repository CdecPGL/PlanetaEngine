#include <cassert>

#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"

#include "InitFunctions.h"
#include "SystemVariables.h"
#include "SystemLog.h"
#include "FileAccessor.h"
#include "EngineConfigData.h"
#include "File.h"
#include "RJson.h"
#include "MakeResource.h"
#include "ProgramDefinitionData.h"


namespace {
	std::ofstream LogFileOutPutStream; //���O�o�͗p�X�g���[��
	namespace prog_def {
		constexpr char* SCENE_SECTION{ "Scene" };
		constexpr char* COLLISION_SECTION{ "Collision" };
		constexpr char* COLLISIONGROUP_SECTION{ "Groups" };
		constexpr char* COLLISIONMATRIX_SECTION{ "CollidableMatrix" };
		constexpr char* STARTUPSCENE_DATA_NAME("Startup");
	}
}

namespace planeta {
	namespace core {
		namespace init_funcs {
			std::tuple<bool, std::function<void()>> InitializeLogSystem() {
				//�J�����[�h�Ȃ�R���\�[���E�C���h�E���쐬���A�����Ƀ��O���o��
				if (system_variables::DevelopmentMode) {
					//�R���\�[���o�͂�L����
					debug::SystemLog::instance().ValidateConsoleOutPut();
				}
				//���O�f�B���N�g�����Ȃ���������
				if(!boost::filesystem::exists(system_variables::file_system::LogDirectory)) {
					boost::filesystem::create_directory(system_variables::file_system::LogDirectory);
				}
				//���O�o�̓t�@�C�����J��
				LogFileOutPutStream.open(system_variables::file_system::LogDirectory + "\\" + system_variables::file_system::LogOutPutFileName, std::ios::out | std::ios::trunc);
				//�V�X�e�����O�o�͐�Ƀt�@�C���o�͂�ǉ�
				debug::SystemLog::instance().AddLogOutStream(LogFileOutPutStream);
				if (debug::SystemLog::instance().Initialize()) {
					return{ true, [] {debug::SystemLog::instance().Finalize(); LogFileOutPutStream.flush(); LogFileOutPutStream.close(); } };
				} else {
					assert(false);
					return{ false,[] {} };
				}
			}
			bool LoadEngineConfig(const std::shared_ptr<FileAccessor> sys_dir_accessor) {
				auto ec_file = sys_dir_accessor->LoadFile(system_variables::file_system::EngineConfigFileName);
				if (ec_file == nullptr) {
					PE_LOG_FATAL("�G���W���ݒ�t�@�C��\"", system_variables::file_system::EngineConfigFileName, "\"���J���܂���ł����B");
					return false;
				}
				if (engine_config::LoadConfigData(std::const_pointer_cast<File>(ec_file)) == false) {
					PE_LOG_FATAL("�G���W���ݒ�t�@�C��\"", system_variables::file_system::EngineConfigFileName, "\"���ǂݍ��߂܂���ł����B");
					return false;
				}
				return true;
			}

			bool LoadProgramDefinition(const std::shared_ptr<FileAccessor> sys_dir_accessor, ProgramDefinitionData* out) {
				auto file = sys_dir_accessor->LoadFile(system_variables::file_system::ProgramDefinitionFileName);
				if (file == nullptr || file->GetStatus() != File::FileStatus::Available) {
					PE_LOG_ERROR("�v���O�����p��`�t�@�C�����J���܂���ł����B");
					return false;
				}
				auto json_res = MakeResource<RJson>();
				//File����INI���\�[�X���쐬����
				if (!json_res->Create(file)) {
					PE_LOG_ERROR("�v���O�����p��`�t�@�C����ǂݍ��߂܂���ł����B");
					return false;
				}
				//�e��ݒ�f�[�^���擾����
				try {
					auto root_obj = json_res->GetRoot().Get<JSONObject>();
					auto scene_obj = root_obj->AtWithException(prog_def::SCENE_SECTION)->GetWithException<JSONObject>();
					auto col_obj = root_obj->AtWithException(prog_def::COLLISION_SECTION)->GetWithException<JSONObject>();
					//�V�[��
					out->startup_scene_id = *scene_obj->AtWithException(prog_def::STARTUPSCENE_DATA_NAME)->GetWithException<std::string>();
					//�Փ˃V�X�e��
					//�Փ˃O���[�v
					std::vector<std::string> group_list;
					group_list = *col_obj->AtWithException(prog_def::COLLISIONGROUP_SECTION)->GetWithException<std::vector<std::string>>();
					out->collision_group_matrix.AddCollisionGroups(group_list);
					//�Փˉ\�}�g���b�N�X
					auto col_mtx = *col_obj->AtWithException(prog_def::COLLISIONMATRIX_SECTION)->GetWithException<std::unordered_map<std::string, std::vector<std::string>>>();
					for (auto&& group : col_mtx) {
						for (auto&& cbl_group : group.second) {
							if (!out->collision_group_matrix.SetCollisionFlag(group.first, cbl_group, true)) {
								PE_LOG_WARNING("�Փ˃O���[�v\"", group.first, "\"��\"", cbl_group, "\"�̃t���O�̐ݒ�Ɏ��s���܂����B�ǂ��炩�̃O���[�v����`����Ă��Ȃ��\��������܂��B");
							}
						}
					}
				} catch (std::out_of_range& e) {
					PE_LOG_ERROR("�v���O�����p��`�̎擾�Ɏ��s���܂����B���e���s�����Ă���\��������܂��B(", e.what(), ")");
					return false;
				} catch (JSONTypeError& e) {
					PE_LOG_ERROR("�v���O�����p��`�̎擾�Ɏ��s���܂����B�^���ԈႦ�Ă���\��������܂��B(", e.what(), ")");
					return false;
				}
				return true;
			}
		}
	}
}
