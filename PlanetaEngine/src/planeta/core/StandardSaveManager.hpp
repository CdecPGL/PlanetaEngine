#pragma once

#include <vector>

#include "save_manager.hpp"
#include "date_time.hpp"

namespace plnt {
	namespace util {
		class data_container;
	}

	namespace private_ {
		class standard_save_manager final : public save_manager {
		public:
			standard_save_manager();
			standard_save_manager(const standard_save_manager &) = delete;
			standard_save_manager(standard_save_manager &&) = delete;
			~standard_save_manager() override;
			standard_save_manager &operator=(const standard_save_manager &) = delete;
			standard_save_manager &operator=(standard_save_manager &&) = delete;

			//ファイルアクセサを設定。初期化前に呼び出す。
			void set_file_manipulator(const std::shared_ptr<file_manipulator> &file_accessor) override;
			bool initialize() override;
			void finalize() override;

			struct user_data_header {
				util::date_time update_time;
				std::vector<std::string> header;
			};

			/*保存*/
			void save() const;
			/*ユーザーデータを読み込み*/
			// NOLINTNEXTLINE(modernize-use-nodiscard)
			bool load_user_data(int idx) const;
			/*ユーザーデータの数を取得*/
			[[nodiscard]] int get_user_data_count() const;
			/*ユーザーデータのヘッダ情報数を取得*/
			[[nodiscard]] int get_user_data_header_param_count() const;
			/*ユーザーデータのヘッダ情報を取得*/
			// TODO
			//[[nodiscard]] const user_data_header &get_user_data_header_param(int idx) const;
			/*共通データを取得*/
			[[nodiscard]] const util::data_container &get_common_data() const;
			/*共通データを取得*/
			[[nodiscard]] util::data_container &get_common_data();
			/*ユーザーデータを取得*/
			[[nodiscard]] boost::optional<const util::data_container &> get_current_data() const;
			/*ユーザーデータを取得*/
			[[nodiscard]] boost::optional<util::data_container &> get_current_data();
			/*ユーザーデータを新規作成(インデックスを返す)*/
			// TODO
			//int create_user_data();
			/*ユーザーデータを複製(インデックスを返す)*/
			// TODO
			//int duplicate_user_data();
			/*ユーザーデータを削除*/
			// TODO
			//bool delete_user_data(int idx);

		private:
			class impl;
			std::unique_ptr<impl> impl_;
		};
	}
}
