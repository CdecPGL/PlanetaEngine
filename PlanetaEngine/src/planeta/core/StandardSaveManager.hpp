﻿#pragma once

#include <vector>
#include "SaveManager.hpp"
#include "date_time.hpp"
#include "boost/optional.hpp"

namespace plnt {
	namespace util {
		class data_container;
	}

	namespace private_ {
		class StandardSaveManager final : public save_manager {
		public:
			StandardSaveManager();
			~StandardSaveManager();
			//ファイルアクセサを設定。初期化前に呼び出す。
			void set_file_manipulator(const std::shared_ptr<file_manipulator> &file_accessor) override;
			bool initialize() override;
			void finalize() override;

			struct UserDataHeader {
				util::date_time update_time;
				std::vector<std::string> header;
			};

			/*保存*/
			void Save();
			/*ユーザーデータを読み込み*/
			bool LoadUserData(int idx);
			/*ユーザーデータの数を取得*/
			int GetUserDataConut() const;
			/*ユーザーデータのヘッダ情報数を取得*/
			int GetUserDataHeaderParamCount() const;
			/*ユーザーデータのヘッダ情報を取得*/
			const UserDataHeader &GetUserDataHeaderParam(int idx) const;
			/*共通データを取得*/
			const util::data_container &GetCommonData() const;
			/*共通データを取得*/
			util::data_container &GetCommonData();
			/*ユーザーデータを取得*/
			boost::optional<const util::data_container &> GetCurrentData() const;
			/*ユーザーデータを取得*/
			boost::optional<util::data_container &> GetCurrentData();
			/*ユーザーデータを新規作成(インデックスを返す)*/
			int CreateUserData();
			/*ユーザーデータを複製(インデックスを返す)*/
			int DuplicateUserData();
			/*ユーザーデータを削除*/
			bool DeleteUserData(int idx);

		private:
			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	}
}
