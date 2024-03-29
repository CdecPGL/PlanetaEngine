﻿#include "system_variables.hpp"

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
		const std::string builtin_graph_resource_type_name("graphic");
		const std::string builtin_sound_resource_type_name("sound_effect");
		const std::string builtin_music_resource_type_name("music");
		const std::string builtin_csv_resource_type_name("csv");
		const std::string builtin_text_resource_type_name("text");
		const std::string builtin_xml_resource_type_name("xml");
		const std::string builtin_font_resource_type_name("font");
		const std::string builtin_json_resource_type_name("json");
		const std::string builtin_game_object_resource_type_name("game_object");
		const std::string builtin_game_object_component_resource_type_name("game_object_component");
		const std::string builtin_effect_resource_type_name("effect");
		const std::string builtin_effect_texture_resource_type_name("effect_texture");

		const std::string builtin_graph_resource_type_prefix("grp");
		const std::string builtin_sound_resource_type_prefix("snd");
		const std::string builtin_music_resource_type_prefix("msc");
		const std::string builtin_csv_resource_type_prefix("csv");
		const std::string builtin_text_resource_type_prefix("txt");
		const std::string builtin_xml_resource_type_prefix("xml");
		const std::string builtin_font_resource_type_prefix("fnt");
		const std::string builtin_json_resource_type_prefix("jsn");
		const std::string builtin_game_object_resource_type_prefix("gbj");
		const std::string builtin_game_object_component_resource_type_prefix("gcm");
		const std::string builtin_effect_resource_type_prefix("efc");
		const std::string builtin_effect_texture_resource_type_prefix("etx");
	}

	namespace prefixes {
		const char scene{'s'};
		const char game_object{'g'};
		const char task{'t'};
		const char game_object_component{'c'};
		const char game_object_interface{'j'};
		const char standard_interface{'i'};
		const char event_arg{'e'};
		const char resource{'r'};
	}

	const bool development_mode(true);
}
