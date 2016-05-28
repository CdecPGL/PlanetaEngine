#include <cassert>

#include "boost/algorithm/string.hpp"

#include "InitFunctions.h"
#include "SystemVariables.h"
#include "SystemLog.h"
#include "FileAccessor.h"
#include "EngineConfigData.h"
#include "File.h"
#include "INIFileResource.h"
#include "MakeResource.h"
#include "ProgramDefinitionData.h"


namespace {
	std::ofstream LogFileOutPutStream; //���O�o�͗p�X�g���[��
	namespace prog_def {
		constexpr char* SCENE_SECTION{ "Scene" };
		constexpr char* COLLISIONGROUP_SECTION{ "CollisionGroup" };
		constexpr char* COLLISIONMATRIX_SECTION{ "CollisionMatrix" };
		constexpr char* STARTUPSCENE_DATA_NAME("StartupScene");
		constexpr char* GROUPS_DATA_NAME("Groups");
	}
}

namespace planeta_engine {
	namespace core {
		namespace init_funcs {
			std::tuple<bool, std::function<void()>> InitializeLogSystem() {
				//�J�����[�h�Ȃ�R���\�[���E�C���h�E���쐬���A�����Ƀ��O���o��
				if (system_variables::DevelopmentMode) {
					//�R���\�[���o�͂�L����
					debug::SystemLog::instance().ValidateConsoleOutPut();
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
				auto ini_res = MakeResource<resources::INIFileResource>();
				//File����INI���\�[�X���쐬����
				if (!ini_res->Create(file)) {
					PE_LOG_ERROR("�v���O�����p��`�t�@�C����ǂݍ��߂܂���ł����B");
					return false;
				}
				//�e��ݒ�f�[�^���擾����
				//�V�[��
				auto s_sec = ini_res->GetSection(prog_def::SCENE_SECTION);
				if (s_sec) {
					try {
						auto startup_scene = s_sec->at(prog_def::STARTUPSCENE_DATA_NAME);
						out->startup_scene_id = startup_scene;
					} catch (std::out_of_range&) {
						PE_LOG_ERROR("�v���O�����p��`�t�@�C���̃V�[���Ɋւ���Z�N�V�����ɁA�X�^�[�g�A�b�v�V�[������`����Ă��܂���B");
						return false;
					}
				} else {
					PE_LOG_ERROR("�v���O�����p��`�t�@�C���ɃV�[���Ɋւ���Z�N�V���������݂��܂���B");
					return false;
				}
				//�Փ˃O���[�v
				std::vector<std::string> group_list;
				auto cg_sec = ini_res->GetSection(prog_def::COLLISIONGROUP_SECTION);
				if (cg_sec) {
					try {
						auto groupes = cg_sec->at(prog_def::GROUPS_DATA_NAME);
						boost::split(group_list, groupes, boost::is_any_of(","));
						out->collision_group_matrix.AddCollisionGroups(group_list);
					}
					catch (std::out_of_range&) {
						PE_LOG_ERROR("�v���O�����p��`�t�@�C���̏Փ˃O���[�v�Ɋւ���Z�N�V�����ɁA�O���[�v���X�g����`����Ă��܂���B");
						return false;
					}
				} else {
					PE_LOG_ERROR("�v���O�����p��`�t�@�C���ɏՓ˃O���[�v�Ɋւ���Z�N�V���������݂��܂���B");
					return false;
				}
				//�Փ˃}�g���b�N�X
				auto cm_sec = ini_res->GetSection(prog_def::COLLISIONMATRIX_SECTION);
				if (cm_sec) {
					for (auto&& group_name : group_list) {
						try {
							auto colables = cm_sec->at(group_name);
							std::vector<std::string> colable_list;
							boost::split(colable_list, colables, boost::is_any_of(","));
							for (auto&& colable_group : colable_list) {
								out->collision_group_matrix.SetCollisionFlag(group_name, colable_group, true);
							}
						} catch (std::out_of_range&) {
							continue;
						}
					}
				} else {
					PE_LOG_ERROR("�v���O�����p��`�t�@�C���ɏՓ˃}�g���b�N�X�Ɋւ���Z�N�V���������݂��܂���B");
					return false;
				}

				return true;
			}
		}
	}
}
