#pragma once

#include<string>

#include "Object.hpp"

namespace plnt {
	/*! ファイルデータを格納するクラス*/
	class file final : public object {
	public:
		/*! 既定のコンストラクタ*/
		file();
		/*! コピーコンストラクタ*/
		file(const file &);
		/*! ムーブコンストラクタ*/
		file(file &&) noexcept;
		/*! データとファイル名を指定するコンストラクタ*/
		file(unsigned char *top_ptr, size_t size, const std::string &file_name);
		/*! データを指定するコンストラクタ*/
		file(unsigned char *top_ptr, size_t size);
		/*! デストラクタ*/
		~file() override;
		/*! 代入演算子*/
		file &operator=(const file &) &;
		/*! ムーブ演算子*/
		file &operator=(file &&) & noexcept;
		/*! データをクリア*/
		void clear();
		/*! サイズを取得*/
		[[nodiscard]] size_t size() const;
		/*! 先頭のポインタを取得(const版)*/
		[[nodiscard]] const unsigned char *top_pointer() const &;
		/*! 先頭のポインタを取得*/
		[[nodiscard]] unsigned char *top_pointer() &;
		/*! 利用可能か*/
		[[nodiscard]] bool is_available() const;
		/*! データの委譲(コピーするのではなく管理を委譲する)*/
		void move_data(unsigned char *top_ptr, size_t size);
		/*! 文字列からデータをセット*/
		void set_data(const std::string &data);
		/*! データの書き込み*/
		bool write_data(size_t pos, const unsigned char *data_top, size_t data_size, bool auto_extend = false);
		/*! データの書き込み(テンプレート版)*/
		template <typename T>
		bool write_data(const size_t pos, const T &d, const bool auto_extend = true) {
			return write_data(pos, reinterpret_cast<const unsigned char *>(&d), sizeof(T), auto_extend);
		}

		/*! データの読み込み*/
		bool read_data(size_t pos, unsigned char *buffer_top, size_t buffer_size) const;
		/*! データの読み込み(テンプレート版)*/
		template <typename T>
		bool read_data(const size_t pos, T &b) const {
			return read_data(pos, reinterpret_cast<unsigned char *>(&b), sizeof(T));
		}

		/*! 領域の確保*/
		bool reserve(size_t size, bool copy = true);
		/*! ファイル名の設定*/
		void set_file_name(const std::string &file_name);
		/*! ファイル名の取得*/
		[[nodiscard]] std::string file_name() const;

	private:
		std::string extension_;
		bool is_available_ = false;
		unsigned char *data_top_ = nullptr;
		size_t size_ = 0;
		std::string file_name_;
	};
}
