#include "planeta/core/SystemVariables.hpp"

namespace plnt{
	namespace private_{
		namespace system_variables{
			namespace engine_information {
				const std::string VersionString("1.0.0");
				const int MajorVersionNumber(1);
				const int MinorVersionNumber(0);
				const int SubVesionNumber(0);
				const std::string BuildDate(__DATE__);
			}
			namespace file_system {
				const std::string SystemConfigFileName("system_config");
				const std::string ProgramDefinitionFileName("prog_def");
				const std::string UserConfigFileName("user_config.json");
				const std::string KeyConfigFileName("key_config.ini");
				const std::string LogOutPutFileName("pe_log.txt");
				const std::string ResourceListFileName("resource_list");
				const std::string ResourceDataArchiveFileName("data");
				const std::string DevResourceDataFolderName("dev_data");

				const std::string SystemDataDirectory("system");
				const std::string LogDirectory("log");
				const std::string SaveDataDirectory("save_datas");
				const std::string ResourceDataDirectory("resources");
				const std::string ConfigFileDirectory("config");

				const unsigned int ResourceDataArchiveDecryptionKey(0);
			}
			namespace resource {
				const std::string BuiltinGraphResourceTypeName("Graphic");
				const std::string BuiltinSoundResourceTypeName("SoundEffect");
				const std::string BuiltinMusicResourceTypeName("Music");
				const std::string BuiltinCSVResourceTypeName("CSV");
				const std::string BuiltinTextResourceTypeName("Text");
				const std::string BuiltinXMLResourceTypeName("XML");
				const std::string BuiltinFontResourceTypeName("Font");
				const std::string BuiltinJSONResourceTypeName("JSON");
				const std::string BuiltinGameObjectResourceTypeName("GameObject");
				const std::string BuiltinGameObjectComponentResourceTypeName("GameObjectComponent");
				const std::string BuiltinEffectResourceTypeName("Effect");
				const std::string BuiltinEffectTextureResourceTypeName("EffectTexture");

				const std::string BuiltinGraphResourceTypePrefix("Grp");
				const std::string BuiltinSoundResourceTypePrefix("Snd");
				const std::string BuiltinMusicResourceTypePrefix("Msc");
				const std::string BuiltinCSVResourceTypePrefix("Csv");
				const std::string BuiltinTextResourceTypePrefix("Txt");
				const std::string BuiltinXMLResourceTypePrefix("Xml");
				const std::string BuiltinFontResourceTypePrefix("Fnt");
				const std::string BuiltinJSONResourceTypePrefix("Jsn");
				const std::string BuiltinGameObjectResourceTypePrefix("Gbj");
				const std::string BuiltinGameObjectComponentResourceTypePrefix("Gcm");
				const std::string BuiltinEffectResourceTypePrefix("Efc");
				const std::string BuiltinEffectTextureResourceTypePrefix("Etx");
			}
			namespace prefixes {
				const char Scene{ 'S' };
				const char GameObject{ 'G' };
				const char Task{ 'T' };
				const char GameObjectComponent{ 'C' };
				const char GameObjectInterface{ 'J' };
				const char StandardInterface{ 'I' };
				const char EventArg{ 'E' };
				const char Resource{ 'R' };
			}
			const bool DevelopmentMode(true);
		}
	}
}