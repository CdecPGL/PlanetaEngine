#pragma once

#include<memory>
#include<string>
#include "Object.h"
#include"File.h"

namespace planeta_engine{
	namespace file_system{
		class EncrypterBase;
		class FileLoaderBase : public core::Object{
		public:
			explicit FileLoaderBase(const std::string& p) :path_(p),is_valid_(false){}
			explicit FileLoaderBase(const std::string& p, const std::shared_ptr<const EncrypterBase>& encrypter) :path_(p), encrypter_(encrypter), is_valid_(false) {}
			virtual ~FileLoaderBase() = default;
			bool Initialize() {
				if (is_valid_) { return false; }
				if (_Initialize()) { is_valid_ = true; return true; }
				else { return false; }
			}
			void Finalize() {
				_Finalize();
				is_valid_ = false;
			}
			/*ファイルの読み込み。キャッシュがある場合はキャッシュを利用*/
			virtual std::shared_ptr<File> LoadFile(const std::string& name) = 0;
			/*キャッシュデータの消去*/
			virtual bool DeleteCache() { return true; }
			/*全ファイルデータをキャッシュとして読み込み(読み込み済みのものはスキップ)*/
			virtual bool LoadAllFileToCache() { return true; }
			/*ファイルリストを更新(キャッシュは全消去されます)*/
			virtual bool UpdateFileList() = 0;
			/*キャッシュされたデータのサイズ*/
			virtual size_t GetCacheSize()const { return 0; }
			bool is_valid()const { return is_valid_; }
		protected:
			virtual bool _Initialize() = 0;
			virtual void _Finalize() = 0;
			const std::string& path()const { return path_; }
			void path(const std::string& p) { path_ = p; }
			bool is_encrypter_valid()const { return encrypter_ != nullptr; }
			std::shared_ptr<const EncrypterBase> encrypter()const { return encrypter_; }
		private:
			std::string path_;
			bool is_valid_; //有効か
			std::shared_ptr<const EncrypterBase> encrypter_;
		};
	}
}
