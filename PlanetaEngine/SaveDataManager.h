#pragma once

#include <memory>
#include <vector>
#include "PointerSingletonTemplate.h"
#include "PEDateTime.h"
#include "boost/optional.hpp"

namespace planeta_engine {
	namespace utility {
		class DataContainer;
	}
	namespace file_system {
		class FileAccessor;
	}
	namespace core {
		class SaveDataManager final: public utility::PointerSingletonTemplate<SaveDataManager>{
			friend class utility::PointerSingletonTemplate<SaveDataManager>;
		public:
			void SetFileAccessor(const std::shared_ptr<file_system::FileAccessor>& file_accessor);
			bool Initialize()override;
			bool Finalize()override;

			struct UserDataHeader {
				utility::DateTime update_time;
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
			const utility::DataContainer& GetCommonData()const;
			/*共通データを取得*/
			utility::DataContainer& GetCommonData();
			/*ユーザーデータを取得*/
			boost::optional<const utility::DataContainer&> GetCurrentData()const;
			/*ユーザーデータを取得*/
			boost::optional<utility::DataContainer&> GetCurrentData();
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