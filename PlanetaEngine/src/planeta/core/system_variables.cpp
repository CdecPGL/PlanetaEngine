#include "system_variables.hpp"

namespace plnt::private_::system_variables {
	namespace engine_information {
		const std::string version_string("1.0.0");
		const int major_version_number(1);
		const int minor_version_number(0);
		const int sub_version_number(0);
		const std::string build_date(__DATE__);
	}

	namespace file_system {
		const std::string system_config_file_name("system_config");
		const std::string program_definition_file_name("prog_def");
		const std::string user_config_file_name("user_config.json");
		const std::string key_config_file_name("key_config.ini");
		const std::string log_out_put_file_name("pe_log.txt");
		const std::string resource_list_file_name("resource_list");
		const std::string resource_data_archive_file_name("data");
		const std::string dev_resource_data_folder_name("dev_data");

		const std::string system_data_directory("system");
		const std::string log_directory("log");
		const std::string save_data_directory("save_datas");
		const std::string resource_data_directory("resources");
		const std::string config_file_directory("config");

		const unsigned int resource_data_archive_decryption_key(0);
	}

	namespace resource {
		const std::string builtin_graph_resource_type_name("Graphic");
		const std::string builtin_sound_resource_type_name("SoundEffect");
		const std::string builtin_music_resource_type_name("Music");
		const std::string builtin_csv_resource_type_name("CSV");
		const std::string builtin_text_resource_type_name("Text");
		const std::string builtin_xml_resource_type_name("XML");
		const std::string builtin_font_resource_type_name("Font");
		const std::string builtin_json_resource_type_name("JSON");
		const std::string builtin_game_object_resource_type_name("GameObject");
		const std::string builtin_game_object_component_resource_type_name("GameObjectComponent");
		const std::string builtin_effect_resource_type_name("Effect");
		const std::string builtin_effect_texture_resource_type_name("EffectTexture");

		const std::string builtin_graph_resource_type_prefix("Grp");
		const std::string builtin_sound_resource_type_prefix("Snd");
		const std::string builtin_music_resource_type_prefix("Msc");
		const std::string builtin_csv_resource_type_prefix("Csv");
		const std::string builtin_text_resource_type_prefix("Txt");
		const std::string builtin_xml_resource_type_prefix("Xml");
		const std::string builtin_font_resource_type_prefix("Fnt");
		const std::string builtin_json_resource_type_prefix("Jsn");
		const std::string builtin_game_object_resource_type_prefix("Gbj");
		const std::string builtin_game_object_component_resource_type_prefix("Gcm");
		const std::string builtin_effect_resource_type_prefix("Efc");
		const std::string builtin_effect_texture_resource_type_prefix("Etx");
	}

	namespace prefixes {
		const char scene{'S'};
		const char game_object{'G'};
		const char task{'T'};
		const char game_object_component{'C'};
		const char game_object_interface{'J'};
		const char standard_interface{'I'};
		const char event_arg{'E'};
		const char resource{'R'};
	}

	const bool development_mode(true);
}
