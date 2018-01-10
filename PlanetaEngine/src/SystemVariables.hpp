#pragma once

#include <string>

namespace plnt{
	namespace private_{
		namespace system_variables{
			namespace engine_information {
				extern const std::string VersionString; //バージョン文字列
				extern const int MajorVersionNumber; //メジャーバージョン番号
				extern const int MinorVersionNumber; //マイナーバージョン番号
				extern const int SubVesionNumber; //サブバージョン番号
				extern const std::string BuildDate; //ビルドした日付
			}
			namespace file_system {
				extern const std::string SystemConfigFileName; //システム設定ファイル名
				extern const std::string ProgramDefinitionFileName; //プログラム用定義ファイル
				extern const std::string UserConfigFileName; //ユーザー設定ファイル名
				extern const std::string KeyConfigFileName; //キー設定ファイル名
				extern const std::string LogOutPutFileName; //ログ出力ファイル名
				extern const std::string ResourceListFileName; //リソースリストファイル名
				extern const std::string ResourceDataArchiveFileName; //リソースデータアーカイブファイル名
				extern const std::string DevResourceDataFolderName; //resource開発用データフォルダ名(開発モード時に有効)

				extern const std::string SystemDataDirectory; //システムデータのディレクトリ
				extern const std::string LogDirectory; //ログのディレクトリ
				extern const std::string SaveDataDirectory; //セーブデータのディレクトリ
				extern const std::string ResourceDataDirectory; //リソースデータディレクトリ
				extern const std::string ConfigFileDirectory; //設定ファイルディレクトリ

				extern const std::string ResourceFileAccessorID; //リソース用ファイルAccessorのID
				extern const std::string SaveDataFileAccessorID; //SameData用ファイルアクセサのID
				extern const std::string SystemFileAccessorID; //System用ファイルAccessorのID
				extern const std::string ConfigFileAccessorID; //Config用ファイルアクセサのID

				extern const unsigned int ResourceDataArchiveDecryptionKey; //リソースデータアーカイブの復号化キー
			}
			namespace resource {
				extern const std::string BuiltinGraphResourceTypeName; //標準の画像リソースタイプ名
				extern const std::string BuiltinSoundResourceTypeName; //標準の効果音リソースタイプ名
				extern const std::string BuiltinMusicResourceTypeName; //標準の音楽リソースタイプ名
				extern const std::string BuiltinCSVResourceTypeName; //標準のCSV形式ファイルリソースタイプ名
				extern const std::string BuiltinTextResourceTypeName; //標準のテキストファイルリソースタイプ名
				extern const std::string BuiltinXMLResourceTypeName; //標準のXMLファイルリソースタイプ名
				extern const std::string BuiltinFontResourceTypeName; //標準のフォント定義ファイルリソースタイプ名
				extern const std::string BuiltinJSONResourceTypeName; //標準のJSONファイルリソースタイプ名
				extern const std::string BuiltinGameObjectResourceTypeName; //標準のGameObjectリソースタイプ名
				extern const std::string BuiltinGameObjectComponentResourceTypeName; //標準のGameObjectComponentリソースタイプ名
				extern const std::string BuiltinEffectResourceTypeName; //標準のEffectリソースタイプ名
				extern const std::string BuiltinEffectTextureResourceTypeName; //標準のEffectテクスチャリソースタイプ名

				extern const std::string BuiltinGraphResourceTypePrefix; //標準の画像リソース接頭辞
				extern const std::string BuiltinSoundResourceTypePrefix; //標準の効果音リソース接頭辞
				extern const std::string BuiltinMusicResourceTypePrefix; //標準の音楽リソース接頭辞
				extern const std::string BuiltinCSVResourceTypePrefix; //標準のCSV形式ファイルリソース接頭辞
				extern const std::string BuiltinTextResourceTypePrefix; //標準のテキストファイルリソース接頭辞
				extern const std::string BuiltinXMLResourceTypePrefix; //標準のXMLファイルリソース接頭辞
				extern const std::string BuiltinFontResourceTypePrefix; //標準のフォント定義ファイルリソース接頭辞
				extern const std::string BuiltinJSONResourceTypePrefix; //標準のJSONファイルリソース接頭辞
				extern const std::string BuiltinGameObjectResourceTypePrefix; //標準のGameObjectリソース接頭辞
				extern const std::string BuiltinGameObjectComponentResourceTypePrefix; //標準のGameObjectComponentリソース接頭辞
				extern const std::string BuiltinEffectResourceTypePrefix; //標準のEffectリソース接頭辞
				extern const std::string BuiltinEffectTextureResourceTypePrefix; //標準のEffectテクスチャリソース接頭辞
			}
			namespace prefixes {
				extern const char Scene;
				extern const char GameObject;
				extern const char Task;
				extern const char GameObjectComponent;
				extern const char GameObjectInterface;
				extern const char StandardInterface;
				extern const char EventArg;
				extern const char Resource;
			}
			/*デバッグ*/
			extern const bool DevelopmentMode; //開発モードフラグ
		}
	}
}