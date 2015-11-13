#pragma once

#include <unordered_map>
#include <memory>
#include "GameDataElement.h"
#include "PointerSingletonTemplate.h"

namespace planeta_engine {
	namespace file_system {
		class FileAccessor;
	}
	namespace core {
		class GameDataManager final : public utility::PointerSingletonTemplate<GameDataManager>{
			friend class utility::PointerSingletonTemplate<GameDataManager>;
		public:
			bool Initialize()override;
			bool Finalize()override;
			/**
			* @brief データを取得する
			* @param データID
			* @return データのスマポ。存在しないときはnullptr
			*/
			std::shared_ptr<GameDataElement> GetElement(const std::string& data_id);
			/**
			* @brief データを設定する
			* @param データのIDとデータの右辺値参照。引数に渡したデータは空になるので注意。戻り値にスマポを返すので引き継図器利用する場合はそれを使う。
			* @brief 引数に渡したデータのスマポ。
			*/
			std::shared_ptr<GameDataElement> SetElement(const std::string& data_id,GameDataElement&& data);
			/**
			* @brief データを保存する
			* @param データIDと同じファイル名で保存する。システムのセーブデータディレクトリに保存する。
			* @return 保存に成功したか否か
			*/
			bool SaveGameData(const std::string& data_id);
			/**
			* @brief データを読み込む
			* @param システムのセーブデータディレクトリの指定名ファイルからデータを読み込む。データIDはファイル名と同じものになる。
			* @return 読み込みに成功したか否か
			*/
			bool LoadGameData(const std::string& file_name);



			static void ResistComplexType(const std::string& type_id,std::unordered_map<std::string,std::string>&& element_types);
		private:
			GameDataManager() = default;
			~GameDataManager() = default;
			std::shared_ptr<file_system::FileAccessor> file_accessor_;
			std::unordered_map<std::string, std::shared_ptr<GameDataElement>> datas_;
			static bool CheckComplexTypeElementTypeError();
		};
	}
}