#pragma once

#include "planeta/Object.hpp"
#include<string>
#include<vector>
#include <memory.h>
#include <istream>

namespace plnt {
	/*! ファイルデータを格納するクラス*/
	class File final: public Object {
	public:
		/*! 既定のコンストラクタ*/
		File();
		/*! コピーコンストラクタ*/
		File(const File&);
		/*! ムーブコンストラクタ*/
		File(File&&);
		/*! データとファイル名を指定するコンストラクタ*/
		File(unsigned char* top_ptr, size_t size,const std::string& file_name);
		/*! データを指定するコンストラクタ*/
		File(unsigned char* top_ptr, size_t size);
		/*! デストラクタ*/
		~File();
		/*! 代入演算子*/
		File& operator=(const File&)&;
		/*! ムーブ演算子*/
		File& operator=(File&&)&;
		/*! データをクリア*/
		void Clear();
		/*! サイズを取得*/
		unsigned int size()const;
		/*! 先頭のポインタを取得(const版)*/
		const unsigned char* top_pointer()const&;
		/*! 先頭のポインタを取得*/
		unsigned char* top_pointer()&;
		/*! 利用可能か*/
		bool is_available()const;
		/*! データの委譲(コピーするのではなく管理を委譲する)*/
		void MoveData(unsigned char* top_ptr, size_t size);
		/*! 文字列からデータをセット*/
		void SetData(const std::string& data);
		/*! データの書き込み*/
		bool WriteData(size_t pos, const unsigned char* data_top, size_t data_size, bool auto_extend = false);
		/*! データの書き込み(テンプレート版)*/
		template<typename T>
		bool WriteData(size_t pos, const T& d, bool auto_extend = true) {
			return WriteData(pos, reinterpret_cast<const unsigned char*>(&d), sizeof(T), auto_extend);
		}
		/*! データの読み込み*/
		bool ReadData(size_t pos, unsigned char* buffer_top, size_t buffer_size)const;
		/*! データの読み込み(テンプレート版)*/
		template<typename T>
		bool ReadData(size_t pos, T& b)const {
			return ReadData(pos, reinterpret_cast<unsigned char*>(&b), sizeof(T));
		}
		/*! 領域の確保*/
		bool Reserve(size_t size, bool copy = true);
		/*! ファイル名の設定*/
		void SetFileName(const std::string& file_name);
		/*! ファイル名の取得*/
		std::string file_name()const;
	private:
		std::string extension_;
		bool is_available_ = false;
		unsigned char* data_top_ = nullptr;
		unsigned int size_ = 0;
		std::string file_name_;
	};
}
