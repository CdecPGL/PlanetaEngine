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
		}
	}
}