#pragma once

#include <memory>
#include <vector>
#include "SingletonTemplate.h"
#include "PEDateTime.h"
#include "boost/optional.hpp"

namespace planeta_engine {
	namespace util {
		class DataContainer;
	}
	class FileAccessor;
	namespace core {
		class SaveDataManager final: public util::SingletonTemplate<SaveDataManager>{
			friend class util::SingletonTemplate<SaveDataManager>;
		public:
			//ファイルアクセサを設定。初期化前に呼び出す。
			void SetFileAccessor_(const std::shared_ptr<FileAccessor>& file_accessor);
			bool Initialize()override;
			void Finalize()override;

			struct UserDataHeader {
				util::DateTime update_time;
				std::vector<std::string> header;
			};
			/*保存*/
			void Save();
			/*ユーザーデータを読み込み*/
			bool LoadUserData(int idx);
			/*ユーザーデータの数を取得*/
			int GetUserDataConut()const;
			/*ユーザーデータのヘッダ情報数を取得*/
			int GetUserDataHeaderParamCount()const;
			/*ユーザーデータのヘッダ情報を取得*/
			const UserDataHeader& GetUserDataHeaderParam(int idx)const;
			/*共通データを取得*/
			const util::DataContainer& GetCommonData()const;
			/*共通データを取得*/
			util::DataContainer& GetCommonData();
			/*ユーザーデータを取得*/
			boost::optional<const util::DataContainer&> GetCurrentData()const;
			/*ユーザーデータを取得*/
			boost::optional<util::DataContainer&> GetCurrentData();
			/*ユーザーデータを新規作成(インデックスを返す)*/
			int CreateUserData();
			/*ユーザーデータを複製(インデックスを返す)*/
			int DuplicateUserData();
			/*ユーザーデータを削除*/
			bool DeleteUserData(int idx);
		private:
			SaveDataManager();
			~SaveDataManager();
			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	}
}