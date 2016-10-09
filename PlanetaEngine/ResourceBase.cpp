#include "ResourceBase.h"
#include "SystemLog.h"
#include "File.h"

namespace planeta {
	namespace private_ {

		ResourceBase::~ResourceBase() {
			if (is_usable_) { PE_LOG_ERROR("リソースの解放が行われていません。(", typeid(*this).name(), ")"); }
		}

		bool ResourceBase::Create(const File& file) {
			if (is_usable_) { 
				PE_LOG_ERROR("読み込み済みのリソースをファイル\"", file.file_name(), "\"から再読み込みしようとしました。リソースタイプは\"",typeid(*this).name(),"\"。");
				return false;
			}
			if (!file.is_available()) {
				PE_LOG_ERROR("無効なファイル\"", file.file_name(), "\"が指定されました。リソースタイプは\"", typeid(*this).name(), "\"。");
				return false;
			}
			if (_Create(file)) {
				is_usable_ = true;
				return true;
			} else { 
				PE_LOG_ERROR("ファイル\"", file.file_name(), "\"からのリソース作成に失敗しました。リソースタイプは\"", typeid(*this).name(), "\"。");
				return false; 
			}
		}

		void ResourceBase::Dispose() {
			if (is_usable_) {
				_Dispose();
				is_usable_ = false;
			} else {
				//破棄処理の重複は許す
				//PE_LOG_ERROR("破棄済みのリソースを破棄しようとしました。リソースタイプは\"", typeid(*this).name(), "\"。");
			}
		}

	}
}