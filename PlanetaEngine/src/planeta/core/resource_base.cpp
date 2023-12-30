#include "boost/filesystem/path.hpp"

#include "resource_base.hpp"
#include "log_utility.hpp"
#include "file.hpp"

namespace plnt {
	resource_base::~resource_base() { if (is_usable_) { PE_LOG_ERROR("リソースの解放が行われていません。(", typeid(*this).name(), ")"); } }

	bool resource_base::load(const file &file, const json_file &metadata,
	                        private_::resource_manager_internal_accessor &mgr_accessor) {
		if (is_usable_) {
			PE_LOG_ERROR("読み込み済みのリソースをファイル\"", file.file_name(), "\"から再読み込みしようとしました。リソースタイプは\"", typeid(*this).name(),
			             "\"。");
			return false;
		}
		if (!file.is_available()) {
			PE_LOG_ERROR("無効なファイル\"", file.file_name(), "\"が指定されました。リソースタイプは\"", typeid(*this).name(), "\"。");
			return false;
		}
		std::vector<std::shared_ptr<resource_base>> ref_list;
		//このリソースの存在するディレクトリパスを求める。
		const std::string ref_path = boost::filesystem::path(file.file_name()).parent_path().string();
		if (resource_referencer referencer{mgr_accessor, ref_path, ref_list}; on_loaded(file, metadata, referencer)) {
			reference_resources_ = std::move(ref_list);
			is_usable_ = true;
			return true;
		}
		PE_LOG_ERROR("ファイル\"", file.file_name(), "\"からのリソース作成に失敗しました。リソースタイプは\"", typeid(*this).name(), "\"。");
		return false;
	}

	void resource_base::dispose() {
		if (is_usable_) {
			on_disposed();
			clear_reference();
			is_usable_ = false;
		} else {
			//破棄処理の重複は許す
			//PE_LOG_ERROR("破棄済みのリソースを破棄しようとしました。リソースタイプは\"", typeid(*this).name(), "\"。");
		}
	}

	size_t resource_base::reference_count() const { return reference_resources_.size(); }

	void resource_base::clear_reference() { reference_resources_.clear(); }
}
