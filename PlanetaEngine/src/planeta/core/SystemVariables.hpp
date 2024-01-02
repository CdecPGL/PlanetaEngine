#pragma once

#include <string>

namespace plnt::private_::system_variables {
	namespace engine_information {
		extern const std::string version_string; //バージョン文字列
		extern const int major_version_number; //メジャーバージョン番号
		extern const int minor_version_number; //マイナーバージョン番号
		extern const int sub_version_number; //サブバージョン番号
		extern const std::string build_date; //ビルドした日付
	}

	namespace file_system {
		extern const std::string system_config_file_name; //システム設定ファイル名
		extern const std::string program_definition_file_name; //プログラム用定義ファイル
		extern const std::string user_config_file_name; //ユーザー設定ファイル名
		extern const std::string key_config_file_name; //キー設定ファイル名
		extern const std::string log_out_put_file_name; //ログ出力ファイル名
		extern const std::string resource_list_file_name; //リソースリストファイル名
		extern const std::string resource_data_archive_file_name; //リソースデータアーカイブファイル名
		extern const std::string dev_resource_data_folder_name; //resource開発用データフォルダ名(開発モード時に有効)

		extern const std::string system_data_directory; //システムデータのディレクトリ
		extern const std::string log_directory; //ログのディレクトリ
		extern const std::string save_data_directory; //セーブデータのディレクトリ
		extern const std::string resource_data_directory; //リソースデータディレクトリ
		extern const std::string config_file_directory; //設定ファイルディレクトリ

		extern const std::string resource_file_accessor_id; //リソース用ファイルAccessorのID
		extern const std::string save_data_file_accessor_id; //SameData用ファイルアクセサのID
		extern const std::string system_file_accessor_id; //System用ファイルAccessorのID
		extern const std::string config_file_accessor_id; //Config用ファイルアクセサのID

		extern const unsigned int resource_data_archive_decryption_key; //リソースデータアーカイブの復号化キー
	}

	namespace resource {
		extern const std::string builtin_graph_resource_type_name; //標準の画像リソースタイプ名
		extern const std::string builtin_sound_resource_type_name; //標準の効果音リソースタイプ名
		extern const std::string builtin_music_resource_type_name; //標準の音楽リソースタイプ名
		extern const std::string builtin_csv_resource_type_name; //標準のCSV形式ファイルリソースタイプ名
		extern const std::string builtin_text_resource_type_name; //標準のテキストファイルリソースタイプ名
		extern const std::string builtin_xml_resource_type_name; //標準のXMLファイルリソースタイプ名
		extern const std::string builtin_font_resource_type_name; //標準のフォント定義ファイルリソースタイプ名
		extern const std::string builtin_json_resource_type_name; //標準のJSONファイルリソースタイプ名
		extern const std::string builtin_game_object_resource_type_name; //標準のGameObjectリソースタイプ名
		extern const std::string builtin_game_object_component_resource_type_name; //標準のGameObjectComponentリソースタイプ名
		extern const std::string builtin_effect_resource_type_name; //標準のEffectリソースタイプ名
		extern const std::string builtin_effect_texture_resource_type_name; //標準のEffectテクスチャリソースタイプ名

		extern const std::string builtin_graph_resource_type_prefix; //標準の画像リソース接頭辞
		extern const std::string builtin_sound_resource_type_prefix; //標準の効果音リソース接頭辞
		extern const std::string builtin_music_resource_type_prefix; //標準の音楽リソース接頭辞
		extern const std::string builtin_csv_resource_type_prefix; //標準のCSV形式ファイルリソース接頭辞
		extern const std::string builtin_text_resource_type_prefix; //標準のテキストファイルリソース接頭辞
		extern const std::string builtin_xml_resource_type_prefix; //標準のXMLファイルリソース接頭辞
		extern const std::string builtin_font_resource_type_prefix; //標準のフォント定義ファイルリソース接頭辞
		extern const std::string builtin_json_resource_type_prefix; //標準のJSONファイルリソース接頭辞
		extern const std::string builtin_game_object_resource_type_prefix; //標準のGameObjectリソース接頭辞
		extern const std::string builtin_game_object_component_resource_type_prefix; //標準のGameObjectComponentリソース接頭辞
		extern const std::string builtin_effect_resource_type_prefix; //標準のEffectリソース接頭辞
		extern const std::string builtin_effect_texture_resource_type_prefix; //標準のEffectテクスチャリソース接頭辞
	}

	namespace prefixes {
		extern const char scene;
		extern const char game_object;
		extern const char task;
		extern const char game_object_component;
		extern const char game_object_interface;
		extern const char standard_interface;
		extern const char event_arg;
		extern const char resource;
	}

	/*デバッグ*/
	extern const bool development_mode; //開発モードフラグ
}
