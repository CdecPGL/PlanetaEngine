#include "SystemVariables.h"

namespace planeta{
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

				const std::string ResourceFileAccessorID("resource");
				const std::string SaveDataFileAccessorID("save_data");
				const std::string SystemFileAccessorID("system");
				const std::string ConfigFileAccessorID("config");

				const unsigned int ResourceDataArchiveDecryptionKey(0);
			}
			namespace resource {
				const std::string BuiltinGraphResourceTypeName("Graphic");
				const std::string BuiltinSoundResourceTypeName("SoundEffect");
				const std::string BuiltinMusicResourceTypeName("Music");
				const std::string BuiltinCSVResourceTypeName("CSV");
				const std::string BuiltinTextResourceTypeName("Text");
				const std::string BuiltinXMLResourceTypeName("XML");
				const std::string BuiltinFontDefinitionResourceTypeName("FontDefinition");
				const std::string BuiltinJSONResourceTypeName("JSON");
				const std::string BuiltinPtreeResourceTypeName("Ptree");
				const std::string BuiltinEffectResourceTypeName("Effect");
				const std::string BuiltinEffectTextureResourceTypeName("EffectTexture");
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