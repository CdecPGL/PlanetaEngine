#include "SystemVariables.h"

namespace planeta_engine{
	namespace core{
		namespace system_variables{
			namespace engine_information {
				const std::string VersionString("1.0.0");
				const int MajorVersionNumber(1);
				const int MinorVersionNumber(0);
				const int SubVesionNumber(0);
				const std::string BuildDate(__DATE__);
			}
			namespace system_file {
				const std::string ConfigFileName("Config");
			}
			const bool DevelopmentMode(true);
			const std::string SystemDataDirectory("system");
			const std::string LogDirectory("log");
			const std::string SaveDataDirectory("save_datas");
			const std::string LogOutPutFileName("pe_log.txt");
			const std::string ResourceDataArchiveFilePath("resources\\data.arc");
			const unsigned int ResourceDataArchiveDecryptionKey(0);
			const std::string DevResourceDataFolderPath("resources\\data");
			const std::string ResourceFileAccessorID("resource");
			const std::string GameDataFileAccessorID("game_data");
			const std::string ResourceListFileName("resource_list.csv");
			const std::string BuiltinGraphResourceTypeName("Graphic");
			const std::string BuiltinSoundResourceTypeName("SoundEffect");
			const std::string BuiltinMusicResourceTypeName("Music");
			const std::string BuiltinCSVResourceTypeName("CSV");
			const std::string BuiltinTextResourceTypeName("Text");
			const std::string BuiltinXMLResourceTypeName("XML");
			const std::string BuiltinFontDefinitionResourceTypeName("FontDefinition");
			namespace process {
				const int GameObjectUpdatetProcessPriority(0); //�Q�[���I�u�W�F�N�g�X�V�v���Z�X�D��x
				const int TransformApplyVelocityProcessPriority(100); //�g�����X�t�H�[�����x�K�p�v���Z�X�D��x
				//const int TransformConvertLocalToGlobalProcessPriority(200); //�g�����X�t�H�[�����[�J���O���[�o���ϊ��v���Z�X�D��x
				const int CollisionDetectProcessPriority(200); //�Փ˔���v���Z�X�D��x
				//const int TransformConvertGlobalToLocalProcessPriority(400);
				const int GameObjectDrawProcessPriority(300); //�Q�[���I�u�W�F�N�g�`��v���Z�X�D��x
				const int UIUpdateProcessPriority(400); //UI�X�V�v���Z�X�D��x
				const int UIDrawProcessPriority(500); //UI�`��v���Z�X�D��x
				const std::string CollisionDetectProcessName("CollisionDetectProcess"); //�Փ˔���v���Z�X��
				const std::string GameObjectDrawProcessName("GameObjectDrawProcess"); //�Q�[���I�u�W�F�N�g�`��v���Z�X��
				const std::string GameObjectUpdatetProcessName("GameObjectUpdateProcess"); //�Q�[���I�u�W�F�N�g�X�V�v���Z�X��
				const std::string UIDrawProcessName("UIDrawProcess"); //UI�`��v���Z�X��
				const std::string UIUpdateProcessName("UIUpdateProcess"); //UI�X�V�v���Z�X��
				const std::string TransformApplyVelocityProcessName("TransformApllyVelocityProcess"); //�g�����X�t�H�[�����x�K�p�v���Z�X��
				//const std::string TransformConvertLocalToGlobalProcessName("TransformConvertLocalToGlobalProcess"); //�g�����X�t�H�[�����[�J���O���[�o���ϊ��v���Z�X��
				//const std::string TransformConvertGlobalToLocalProcessName("TransformConvertGlobalToLocalProcess"); //�g�����X�t�H�[���O���[�o�����[�J���ϊ��v���Z�X��
			}
		}
	}
}