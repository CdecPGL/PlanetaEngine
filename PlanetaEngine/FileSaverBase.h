#pragma once

#include <memory>
#include "Object.h"
#include "File.h"

namespace planeta_engine {
	namespace file_system {
		class EncrypterBase;
		class FileSaverBase : public core::Object{
		public:
			explicit FileSaverBase(const std::string& p,bool auto_create) :path_(p), is_valid_(false),auto_create_(auto_create) {}
			FileSaverBase(const std::string& p, const std::shared_ptr<const EncrypterBase>& encrypter, bool auto_create) :path_(p), is_valid_(false), encrypter_(encrypter), auto_create_(auto_create) {}

			virtual ~FileSaverBase() = default;
			bool Initialize() {
				if (is_valid_) { return false; }
				if (_Initialize()) { is_valid_ = true; return true; }
				else { return false; }
			}
			void Finalize() {
				_Finalize();
				is_valid_ = false;
			}
			/*ファイルの保存(保存名、ファイル)*/
			virtual bool SaveFile(const std::string& name,const std::shared_ptr<File>& file) = 0;
			/*キャッシュデータの消去*/
			virtual bool DeleteCache() { return true; };
			/*全キャッシュデータを保存*/
			virtual bool SaveAllCache() { return true; };
			/*キャッシュされたデータのサイズ*/
			virtual size_t GetCacheSize()const { return 0; };
			bool is_valid()const { return is_valid_; }
		protected:
			virtual bool _Initialize() = 0;
			virtual void _Finalize() = 0;
			const std::string& path()const { return path_; }
			void path(const std::string& p) { path_ = p; }
			bool auto_create()const { return auto_create_; }
			bool is_encrypter_valid()const { return encrypter_ != nullptr; }
			std::shared_ptr<const EncrypterBase> encrypter()const { return encrypter_; }
		private:
			std::string path_;
			bool is_valid_; //有効か
			bool auto_create_;
			std::shared_ptr<const EncrypterBase> encrypter_;
		};
	}
}