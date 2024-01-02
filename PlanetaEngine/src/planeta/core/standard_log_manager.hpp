#pragma once

#include <string>
#include <fstream>
#include <memory>

#include "log_manager.hpp"

namespace plnt::private_ {
	//システムログ管理(スタティックシングルトン)
	class standard_log_manager final : public log_manager {
	public:
		standard_log_manager();
		standard_log_manager(const standard_log_manager &) = delete;
		standard_log_manager(standard_log_manager &&) = delete;
		~standard_log_manager() override;
		standard_log_manager &operator=(const standard_log_manager &) = delete;
		standard_log_manager &operator=(standard_log_manager &&) = delete;

		bool initialize() override;
		void finalize() override;
		/*コンソールへの出力を有効化*/
		bool validate_console_out_put();
		/*ファイルへの出力を追加*/
		// NOLINTNEXTLINE(modernize-use-nodiscard)
		bool validate_file_out_put(const std::string &file_name) const;

		/*ログ出力ストリームを追加(インスタンスの生成、破棄は外部で行う)*/
		void add_log_out_stream(std::ostream &o_strm) const;
		/*ログ出力ストリームをすべて消す*/
		void reset_log_out_stream() const;
		/*ログ履歴を指定されたファイルに出力(エンジンのファイルシステムは使わず、カレントディレクトリに指定された名前で直接出力する)*/
		bool dump_log_history(const std::string &file_name);
		/*ログ履歴最大サイズを設定(0で無限)*/
		void set_log_history_max_size(size_t size);

		enum class dump_level { all, message, warning, error };

		/*出力レベルを設定*/
		//void set_dump_level();

	private:
		class impl;
		std::unique_ptr<impl> impl_;

		void log_proc(log_level level, const std::string &detail, const std::string &place) override;
		void simple_log_proc(const std::string &detail) override;
		auto out_put(log_level level, const std::string &str) const -> void;
	};
}
