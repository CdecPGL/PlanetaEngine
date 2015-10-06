#pragma once

#include<memory>
#include<string>
#include"File.h"

namespace planeta_engine{
	namespace file_system{
		class LoaderBase{
		public:
			LoaderBase(const std::string& p) :_path(p),_is_valid(false){}
			virtual ~LoaderBase() = default;
			bool Initialize() {
				if (_is_valid) { return false; }
				if (_Initialize()) { _is_valid = true; return true; }
				else { return false; }
			}
			void Finalize() {
				_Finalize();
				_is_valid = false;
			}
			/*ファイルの読み込み。キャッシュがある場合はキャッシュを利用*/
			virtual std::shared_ptr<File> LoadFile(const std::string&) = 0;
			/*キャッシュデータの消去*/
			virtual int DeleteCache() = 0;
			/*全ファイルデータをキャッシュとして読み込み(読み込み済みのものはスキップ)*/
			virtual int LoadAllFileToCache() = 0;
			/*ファイルリストを更新(キャッシュは全消去されます)*/
			virtual int UpdateFileList() = 0;
			/*キャッシュされたデータのサイズ*/
			virtual unsigned int GetCacheSize()const = 0;
			bool IsValid()const { return _is_valid; }
		protected:
			virtual bool _Initialize() = 0;
			virtual void _Finalize() = 0;
			std::string _path;
		private:
			bool _is_valid; //有効か
		};
	}
}
