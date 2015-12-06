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
				const int GameObjectUpdatetProcessPriority(0); //ゲームオブジェクト更新プロセス優先度
				const int TransformApplyVelocityProcessPriority(100); //トランスフォーム速度適用プロセス優先度
				//const int TransformConvertLocalToGlobalProcessPriority(200); //トランスフォームローカルグローバル変換プロセス優先度
				const int CollisionDetectProcessPriority(200); //衝突判定プロセス優先度
				//const int TransformConvertGlobalToLocalProcessPriority(400);
				const int GameObjectDrawProcessPriority(300); //ゲームオブジェクト描画プロセス優先度
				const int UIUpdateProcessPriority(400); //UI更新プロセス優先度
				const int UIDrawProcessPriority(500); //UI描画プロセス優先度
				const std::string CollisionDetectProcessName("CollisionDetectProcess"); //衝突判定プロセス名
				const std::string GameObjectDrawProcessName("GameObjectDrawProcess"); //ゲームオブジェクト描画プロセス名
				const std::string GameObjectUpdatetProcessName("GameObjectUpdateProcess"); //ゲームオブジェクト更新プロセス名
				const std::string UIDrawProcessName("UIDrawProcess"); //UI描画プロセス名
				const std::string UIUpdateProcessName("UIUpdateProcess"); //UI更新プロセス名
				const std::string TransformApplyVelocityProcessName("TransformApllyVelocityProcess"); //トランスフォーム速度適用プロセス名
				//const std::string TransformConvertLocalToGlobalProcessName("TransformConvertLocalToGlobalProcess"); //トランスフォームローカルグローバル変換プロセス名
				//const std::string TransformConvertGlobalToLocalProcessName("TransformConvertGlobalToLocalProcess"); //トランスフォームグローバルローカル変換プロセス名
			}
		}
	}
}