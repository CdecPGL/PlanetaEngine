#include "InitEnd.h"
#include <windows.h>
#include <fstream>
#include <iostream>

#include "boost/filesystem.hpp"

#include "SystemVariables.h"
#include "SystemLog.h"
#include "SystemCounter.h"
#include "FileSystemManager.h"
#include "ArchiveManipulator.h"
#include "NormalFolderManipulator.h"
#include "ResourceManager.h"
#include "DrawManager.h"
#include "SoundManager.h"
#include "FileAccessMode.h"

namespace {
	//variables
	std::ofstream LogFileOutPutStream;
}

namespace planeta_engine{
	namespace core{
		namespace init_end{
			bool SetUpSingletonManagers() {
				if (
					SetUpFileSystem()
					//���\�[�X�}�l�[�W���̐ݒ�
					&& SetUpResourceManager()
					) {
					debug::SystemLog::instance().LogMessage("�V���O���g���}�l�[�W���̐ݒ�ɐ������܂����B", __FUNCTION__);
					return true;
				}
				else { 
					debug::SystemLog::instance().LogError("�V���O���g���}�l�[�W���̐ݒ�Ɏ��s���܂����B", __FUNCTION__);
					return false;
				}
			}

			bool InstantiateSingletonManagers() {
				if (
					file_system::FileSystemManager::Instantiate()
					&& core::ResourceManager::Instantiate()
					&& core::DrawManager::Instantiate()
					&& core::SoundManager::Instantiate()
					) {
					debug::SystemLog::instance().LogMessage("�V���O���g���}�l�[�W���̃C���X�^���X���ɐ������܂����B", __FUNCTION__);
					return true;
				}
				else {
					debug::SystemLog::instance().LogError("�V���O���g���}�l�[�W���̃C���X�^���X���Ɏ��s���܂����B", __FUNCTION__);
					return false;
				}
			}

			bool DisposeSingletonManagers() {
				core::SoundManager::Dispose();
				core::DrawManager::Dispose();
				core::ResourceManager::Dispose();
				file_system::FileSystemManager::Dispose();
				debug::SystemLog::instance().LogMessage("�V���O���g���}�l�[�W���̃C���X�^���X��j�����܂����B", __FUNCTION__);
				return true;
			}

			bool InitializeSingletonManagers() {
				if (
					file_system::FileSystemManager::instance().Initialize()
					&& core::ResourceManager::instance().Initialize()
					&& core::DrawManager::instance().Initialize()
					&& core::SoundManager::instance().Initialize()
					) {
					debug::SystemLog::instance().LogMessage("�V���O���g���}�l�[�W���̏������ɐ������܂����B", __FUNCTION__);
					return true;
				}
				else {
					debug::SystemLog::instance().LogError("�V���O���g���}�l�[�W���̏������Ɏ��s���܂����B",__FUNCTION__);
					return false;
				}
			}

			bool FinalizeSingletonManagers() {
				//�T�E���h�}�l�[�W���̏I��
				core::SoundManager::instance().Finalize();
				//�`��}�l�[�W���̏I��
				core::DrawManager::instance().Finalize();
				//���\�[�X�}�l�[�W���̏I��
				core::ResourceManager::instance().Finalize();
				//�t�@�C���V�X�e���̏I��
				file_system::FileSystemManager::instance().Finalize();
				debug::SystemLog::instance().LogMessage("�V���O���g���}�l�[�W���̏I�����������s���܂����B", __FUNCTION__);
				return true;
			}

			bool SetUpSystemLog()
			{
				//�J�����[�h�Ȃ�R���\�[���E�C���h�E���쐬���A�����Ƀ��O���o��
				if (system_variables::DevelopmentMode){
					//�R���\�[���E�C���h�E��W�����o�͂Ɋ��蓖��
					AllocConsole();
					FILE* fp;
					freopen_s(&fp, "CONOUT$", "w", stdout);
					freopen_s(&fp, "CONIN$", "r", stdin);
					//�V�X�e�����O�o�͐�ɕW�����o�͂�ǉ�
					debug::SystemLog::instance().AddLogOutStream(std::cout);
				}
				//���O�o�̓t�@�C�����J��
				LogFileOutPutStream.open(system_variables::LogDirectory + "\\" + system_variables::LogOutPutFileName, std::ios::out | std::ios::trunc);
				//�V�X�e�����O�o�͐�Ƀt�@�C���o�͂�ǉ�
				debug::SystemLog::instance().AddLogOutStream(LogFileOutPutStream);
				return true;
			}

			bool FinalizeSystemLog()
			{
				debug::SystemLog::instance().LogMessage("�V�X�e�����O���I�����܂��B", __FUNCTION__);
				//�I��
				debug::SystemLog::instance().Finalize();
				//���O�t�@�C�������
				LogFileOutPutStream.close();
				if (system_variables::DevelopmentMode) {
					//�R���\�[�����
					FreeConsole();
				}
				return true;
			}

			bool InitializeDebugSystem()
			{
				if(debug::SystemCounter::Instantiate()
					&& debug::SystemLog::Instantiate()) {
				}
				else { return false; }
				//�V�X�e�����O�̐ݒ�
				if (SetUpSystemLog() == false) { return false; }
				if(debug::SystemCounter::instance().Initialize()
					&& debug::SystemLog::instance().Initialize()) {
					debug::SystemLog::instance().LogMessage("�V�X�e�����O���J�n���܂����B", __FUNCTION__);
				}
				else { return false; }
				return true;
			}

			bool FinalizeDebugStstem()
			{
				//�V�X�e�����O�̏I������
				FinalizeSystemLog();
				//�V�X�e���J�E���^�̏I������
				debug::SystemCounter::instance().Finalize();
				//�j��
				debug::SystemLog::Dispose();
				debug::SystemCounter::Dispose();
				return true;
			}

			bool SetUpFileSystem()
			{
				using namespace file_system;
				FileSystemManager& flm = FileSystemManager::instance();
				//���\�[�X�p�t�@�C���A�N�Z�T�ݒ�
				flm.CreateFileAccessor(system_variables::ResourceFileAccessorID, std::make_shared<NormalFolderManipulator>(system_variables::DevResourceDataFolderPath,false),file_system::AccessMode::ReadOnly);
				//GameData�p�t�@�C���A�N�Z�T�ݒ�
				flm.CreateFileAccessor(system_variables::GameDataFileAccessorID, std::make_shared<NormalFolderManipulator>(system_variables::SaveDataDirectory,true), file_system::AccessMode::ReadWrite);

				debug::SystemLog::instance().Log(debug::LogLevel::Message, __FUNCTION__, "�t�@�C���V�X�e���̐ݒ���s���܂����B");
				//���\�[�X�}�l�[�W���̐ݒ���s��
				core::ResourceManager::instance().SetResourceListFileName(core::system_variables::ResourceListFileName);
				return true;
			}

			void CreateSystemDirectory()
			{
				//���O�p�t�H���_�쐬
				boost::filesystem::create_directory(boost::filesystem::path(system_variables::LogDirectory));
				//�V�X�e���f�[�^�p�t�H���_�쐬
				boost::filesystem::create_directory(boost::filesystem::path(system_variables::SystemDataDirectory));
			}
		}
	}
}