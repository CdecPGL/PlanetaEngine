#include "InitEnd.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include "SystemVariables.h"
#include "SystemLog.h"
#include "SystemCounter.h"
#include "FileLoadManager.h"
#include "ArchiveLoader.h"
#include "NormalFolderLoader.h"
#include "ResourceManager.h"
#include "DrawManager.h"
#include "SoundManager.h"

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
					debug::SystemLog::instance().LogMessage("�V���O���g���}�l�[�W���̐ݒ�ɐ������܂����B", "SetUpSingletoneManagers");
					return true;
				}
				else { 
					debug::SystemLog::instance().LogError("�V���O���g���}�l�[�W���̐ݒ�Ɏ��s���܂����B", "SetUpSingletoneManagers");
					return false;
				}
			}

			bool InstantiateSingletonManagers() {
				if (
					file_system::FileLoadManager::Instantiate()
					&& core::ResourceManager::Instantiate()
					&& core::DrawManager::Instantiate()
					&& core::SoundManager::Instantiate()
					) {
					debug::SystemLog::instance().LogMessage("�V���O���g���}�l�[�W���̃C���X�^���X���ɐ������܂����B", "InstantiateSingletonManagers");
					return true;
				}
				else {
					debug::SystemLog::instance().LogError("�V���O���g���}�l�[�W���̃C���X�^���X���Ɏ��s���܂����B", "InstantiateSingletonManagers");
					return false;
				}
			}

			bool DisposeSingletonManagers() {
				core::SoundManager::Dispose();
				core::DrawManager::Dispose();
				core::ResourceManager::Dispose();
				file_system::FileLoadManager::Dispose();
				debug::SystemLog::instance().LogMessage("�V���O���g���}�l�[�W���̃C���X�^���X��j�����܂����B", "DisposeSingletonManagers");
				return true;
			}

			bool InitializeSingletonManagers() {
				if (
					file_system::FileLoadManager::instance().Initialize()
					&& core::ResourceManager::instance().Initialize()
					&& core::DrawManager::instance().Initialize()
					&& core::SoundManager::instance().Initialize()
					) {
					debug::SystemLog::instance().LogMessage("�V���O���g���}�l�[�W���̏������ɐ������܂����B", "InitializeSingletonManagers");
					return true;
				}
				else {
					debug::SystemLog::instance().LogError("�V���O���g���}�l�[�W���̏������Ɏ��s���܂����B", "InitializeSingletonManagers");
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
				file_system::FileLoadManager::instance().Finalize();
				debug::SystemLog::instance().LogMessage("�V���O���g���}�l�[�W���̏I�����������s���܂����B", "FinalizeSingletonManagers");
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
				LogFileOutPutStream.open(system_variables::LogOutPutFileName);
				//�V�X�e�����O�o�͐�Ƀt�@�C���o�͂�ǉ�
				debug::SystemLog::instance().AddLogOutStream(LogFileOutPutStream);
				return true;
			}

			bool FinalizeSystemLog()
			{
				debug::SystemLog::instance().LogMessage("�V�X�e�����O���I�����܂��B", "FinalizeSystemLog");
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
					debug::SystemLog::instance().LogMessage("�V�X�e�����O���J�n���܂����B", "InitializeDebugSystem");
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
				FileLoadManager& flm = FileLoadManager::instance();
				flm.PushLoader(std::make_shared<ArchiveLoader>(system_variables::ResourceDataArchiveFilePath, system_variables::ResourceDataArchiveDecryptionKey));
				//�J�����[�h���́A�J���pResource�t�H���_�ƁA�e�X�g�pResource�t�H���_���ǉ�����B
				bool dev_mode = false;
				if (system_variables::DevelopmentMode) {
					flm.PushLoader(std::make_shared<NormalFolderLoader>(system_variables::DevResourceDataFolderPath));
					flm.PushLoader(std::make_shared<NormalFolderLoader>(system_variables::TestResourceDataFolderPath));
					dev_mode = true;
				}
				char str[256];
				sprintf_s(str, 256, "�t�@�C���V�X�e���̐ݒ���s���܂����B(DevelopmentMode=%s)", dev_mode ? "true" : "false");
				debug::SystemLog::instance().LogMessage(str, "InitializeDebugSystem");
				//���\�[�X�}�l�[�W���̐ݒ���s��
				core::ResourceManager::instance().SetResourceListFileName(core::system_variables::ResourceListFileName);
				return true;
			}
		}
	}
}