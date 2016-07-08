#include <cassert>

#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"

#include "InitFunctions.h"
#include "SystemVariables.h"
#include "SystemLog.h"
#include "FileAccessor.h"
#include "EngineConfigData.h"
#include "File.h"
#include "JSONResource.h"
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
				using namespace resources;
				auto file = sys_dir_accessor->LoadFile(system_variables::file_system::ProgramDefinitionFileName);
				if (file == nullptr || file->GetStatus() != File::FileStatus::Available) {
					PE_LOG_ERROR("�v���O�����p��`�t�@�C�����J���܂���ł����B");
					return false;
				}
				auto json_res = MakeResource<resources::JSONResource>();
				//File����INI���\�[�X���쐬����
				if (!json_res->Create(file)) {
					PE_LOG_ERROR("�v���O�����p��`�t�@�C����ǂݍ��߂܂���ł����B");
					return false;
				}
				//�e��ݒ�f�[�^���擾����
				//�V�[��
				auto root_obj = json_res->GetRoot().Get<JSONObject>();
				if (!root_obj) {
					PE_LOG_ERROR("�v���O�����p��`�t�@�C���Ƀ��[�g�I�u�W�F�N�g�����݂��܂���B");
					return false;
				}
				auto s_obj = root_obj->At<JSONObject>(prog_def::SCENE_SECTION);
				if (s_obj) {
					auto startup_scene = s_obj->At<std::string>(prog_def::STARTUPSCENE_DATA_NAME);
					if (startup_scene) {
						out->startup_scene_id = *startup_scene;
					} else {
						PE_LOG_ERROR("�v���O�����p��`�t�@�C���̃V�[���Ɋւ���I�u�W�F�N�g�ɁA������^�̒l�X�^�[�g�A�b�v�V�[������`����Ă��܂���B");
						return false;
					}
				} else {
					PE_LOG_ERROR("�v���O�����p��`�t�@�C���ɃV�[���Ɋւ���I�u�W�F�N�g�����݂��܂���B");
					return false;
				}
				//�Փ˃V�X�e��
				std::vector<std::string> group_list;
				auto c_obj = root_obj->At<JSONObject>(prog_def::COLLISION_SECTION);
				if (c_obj) {
					//�Փ˃O���[�v���X�g
					auto g_ary = c_obj->At<JSONArray>(prog_def::COLLISIONGROUP_SECTION);
					if (g_ary) {
						for (size_t i = 0; i < g_ary->size(); ++i) {
							auto group_name = g_ary->At<std::string>(i);
							if (group_name) {
								group_list.push_back(*group_name);
							} else {
								PE_LOG_ERROR("�Փ˃O���[�v���X�g", i, "�Ԗڂ̌^���s���ł��B");
								return false;
							}
						}
						out->collision_group_matrix.AddCollisionGroups(group_list);
					} else {
						PE_LOG_ERROR("�v���O�����p��`�t�@�C���ɃR���W�����V�X�e���Ɋւ���I�u�W�F�N�g�ɁA�z��^�̒l�Փ˃O���[�v����`����Ă��܂���B");
						return false;
					}
					//�Փˉ\�}�g���b�N�X
					auto cm_obj = c_obj->At<JSONObject>(prog_def::COLLISIONMATRIX_SECTION);
					if (cm_obj) {
						for (auto&& group_name : group_list) {
							auto cgl = cm_obj->At<JSONArray>(group_name);
							if (cgl) { //�Ȃ��ꍇ������̂łȂ��Ă��G���[�͏o���Ȃ��B
								std::vector<std::string> colable_list;
								for (size_t i = 0; i < cgl->size(); ++i) {
									auto cgn = cgl->At<std::string>(i);
									if (cgn) {
										colable_list.push_back(*cgn);
									} else {
										//�G���[�ɂ͂��Ȃ��B
										PE_LOG_ERROR("�Փ˃O���[�v\"", group_name, "\"�̏Փˉ\�z��ɂ�����", i, "�Ԗڂ̗v�f�̌^���s���ł��B");
									}
								}
								for (auto&& colable_group : colable_list) {
									out->collision_group_matrix.SetCollisionFlag(group_name, colable_group, true);
								}
							}
						}
					} else {
						PE_LOG_ERROR("�v���O�����p��`�t�@�C���ɃR���W�����V�X�e���Ɋւ���I�u�W�F�N�g�ɁA�Փˉ\�}�g���b�N�X�I�u�W�F�N�g����`����Ă��܂���B");
						return false;
					}
				} else {
					PE_LOG_ERROR("�v���O�����p��`�t�@�C���ɃR���W�����V�X�e���Ɋւ���I�u�W�F�N�g�����݂��܂���B");
					return false;
				}
				return true;
			}
		}
	}
}
