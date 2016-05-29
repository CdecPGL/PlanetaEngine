#pragma once

#include <string>

namespace planeta_engine{
	namespace core{
		namespace system_variables{
			namespace engine_information {
				extern const std::string VersionString; //バージョン文字列
				extern const int MajorVersionNumber; //メジャーバージョン番号
				extern const int MinorVersionNumber; //マイナーバージョン番号
				extern const int SubVesionNumber; //サブバージョン番号
				extern const std::string BuildDate; //ビルドした日付
			}
			namespace file_system {
				extern const std::string EngineConfigFileName; //エンジン設定ファイル名
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
				extern const std::string BuiltinFontDefinitionResourceTypeName; //標準のフォント定義ファイルリソースタイプ名
			}
			namespace prefixes {
				extern const std::string Scene;
				extern const std::string GameObject;
				extern const std::string Task;
			}
			/*デバッグ*/
			extern const bool DevelopmentMode; //開発モードフラグ
		}
	}
}