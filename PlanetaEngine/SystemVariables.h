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
			namespace system_file {
				extern const std::string ConfigFileName; //設定ファイル名
			}
			/*デバッグ*/
			extern const bool DevelopmentMode; //開発モードフラグ
			/*ディレクトリ*/
			extern const std::string SystemDataDirectory; //システムデータのディレクトリ
			extern const std::string LogDirectory; //ログのディレクトリ
			extern const std::string SaveDataDirectory; //セーブデータのディレクトリ
			/*システムログ*/
			extern const std::string LogOutPutFileName; //ログ出力先ファイル名
			/*ファイルシステム*/
			extern const std::string ResourceDataArchiveFilePath; //リソースデータアーカイブファイルのパス
			extern const unsigned int ResourceDataArchiveDecryptionKey; //リソースデータアーカイブの復号化キー
			extern const std::string DevResourceDataFolderPath; //resource開発用データフォルダのパス(開発モード時に有効)
			extern const std::string ResourceFileAccessorID; //リソース用ファイルAccessorのID
			extern const std::string GameDataFileAccessorID; //GameData用ファイルアクセサのID
			/*リソース*/
			extern const std::string ResourceListFileName; //リソースリストファイルタイプ名
			extern const std::string BuiltinGraphResourceTypeName; //標準の画像リソースタイプ名
			extern const std::string BuiltinSoundResourceTypeName; //標準の効果音リソースタイプ名
			extern const std::string BuiltinMusicResourceTypeName; //標準の音楽リソースタイプ名
			extern const std::string BuiltinCSVResourceTypeName; //標準のCSV形式ファイルリソースタイプ名
			extern const std::string BuiltinTextResourceTypeName; //標準のテキストファイルリソースタイプ名
			extern const std::string BuiltinXMLResourceTypeName; //標準のXMLファイルリソースタイプ名
			extern const std::string BuiltinFontDefinitionResourceTypeName; //標準のフォント定義ファイルリソースタイプ名
			/*特殊プロセス優先度*/
			namespace process{
				extern const int CollisionDetectProcessPriority; //衝突判定プロセス優先度
				extern const int GameObjectDrawProcessPriority; //ゲームオブジェクト描画プロセス優先度
				extern const int GameObjectUpdatetProcessPriority; //ゲームオブジェクト更新プロセス優先度
				extern const int UIDrawProcessPriority; //UI描画プロセス優先度
				extern const int UIUpdateProcessPriority; //UI更新プロセス優先度
				extern const int TransformApplyVelocityProcessPriority; //トランスフォーム速度適用プロセス優先度
				//extern const int TransformConvertLocalToGlobalProcessPriority; //トランスフォームローカルグローバル変換プロセス優先度
				//extern const int TransformConvertGlobalToLocalProcessPriority; //トランスフォームグローバルローカル変換プロセス優先度
				extern const std::string CollisionDetectProcessName; //衝突判定プロセス名
				extern const std::string GameObjectDrawProcessName; //ゲームオブジェクト描画プロセス名
				extern const std::string GameObjectUpdatetProcessName; //ゲームオブジェクト更新プロセス名
				extern const std::string UIDrawProcessName; //UI描画プロセス名
				extern const std::string UIUpdateProcessName; //UI更新プロセス名
				extern const std::string TransformApplyVelocityProcessName; //トランスフォーム速度適用プロセス名
				//extern const std::string TransformConvertLocalToGlobalProcessName; //トランスフォームローカルグローバル変換プロセス名
				//extern const std::string TransformConvertGlobalToLocalProcessName; //トランスフォームグローバルローカル変換プロセス名
			}
		}
	}
}