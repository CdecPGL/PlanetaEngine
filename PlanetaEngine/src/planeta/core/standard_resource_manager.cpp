#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"

#include"standard_resource_manager.hpp"

#include <ranges>

#include "file.hpp"
#include "json_file.hpp"
#include "log_utility.hpp"
#include "file_manipulator.hpp"
#include "resource_referencer.hpp"
#include "resource_base.hpp"

namespace {
	std::string unify_path(const std::string &src) { return boost::algorithm::replace_all_copy(src, "/", "\\"); }
}

namespace plnt::private_ {
	namespace {
		constexpr char *meta_data_file_suffix{const_cast<char *>(".meta.json")};
		constexpr unsigned int resource_file_name_suffix_length{3};
		constexpr char *tag_list_file_name{const_cast<char *>("tag_list.json")};
	}

	std::shared_ptr<resource_base> standard_resource_manager::create_resource(const std::type_index &type) {
		const auto it = resource_type_data_map_.find(type);
		if (it == resource_type_data_map_.end()) { return nullptr; }
		//指定タイプのリソース作成
		auto res = it->second.creator();
		if (res == nullptr) {
			PE_LOG_ERROR("リソースのインスタンス作成に失敗しました。");
			return nullptr;
		}
		return res;
	}

	std::shared_ptr<resource_base> standard_resource_manager::load_resource(resource_data &res_dat) {
		assert(res_dat.is_loaded == false);
		assert(res_dat.resource == nullptr);
		//リソースファイルを読み込み
		const auto file = file_accessor_->load_file(res_dat.file_path);
		if (file == nullptr) {
			PE_LOG_ERROR("リソースの読み込みに失敗しました。ファイルを読み込めませんでした。");
			return nullptr;
		}
		//メタデータを読み込み
		const auto metadata = std::make_unique<json_file>();
		if (res_dat.has_metadata) {
			const auto metadata_file = file_accessor_->load_file(res_dat.metadata_file_path);
			if (metadata_file == nullptr) { PE_LOG_MESSAGE(res_dat.file_path, "のメタデータファイルは存在しません。"); } else if (!
				metadata->load(*metadata_file)) {
				PE_LOG_ERROR("メタデータファイルの読み込みに失敗しました。Jsonファイルとして読み込めませんでした。");
				return nullptr;
			}
		}
		//マネージャアクセサ作成
		resource_manager_internal_accessor mgr_accessor{create_internal_manager_accessor()};
		//インスタンス作成と初期化
		auto resource = create_resource(res_dat.type_index);
		if (resource->load(*file, *metadata, mgr_accessor) == false) { resource.reset(); }
		if (resource == nullptr) {
			PE_LOG_ERROR("リソースの読み込みに失敗しました。リソースの作成でエラーが発生しました。");
			return nullptr;
		}
		res_dat.is_loaded = true;
		res_dat.resource = resource;
		return resource;
	}

	void standard_resource_manager::unload_resource(resource_data &res_dat) {
		assert(res_dat.is_loaded);
		assert(res_dat.resource != nullptr);
		res_dat.resource->dispose();
		res_dat.resource.reset();
		res_dat.is_loaded = false;
	}

	bool standard_resource_manager::register_resource_data(resource_data &&res_dat) {
		resource_data_list_.push_back(std::move(res_dat));
		const auto it = --resource_data_list_.end();
		//タグマップに登録
		for (auto &&tag : it->tags) { tag_map_[tag].push_back(it); }
		//フルIDマップに登録
		auto full_id_map_it = full_id_map_.find(it->full_id);
		if (full_id_map_.contains(it->full_id)) {
			PE_LOG_ERROR("FullID\"", it->full_id, "\"が重複登録されました。(パス:", it->file_path, ")");
			return false;
		}
		full_id_map_[it->full_id] = it;
		//Pathマップに登録
		if (path_map_.contains(it->full_id)) {
			PE_LOG_ERROR("パス\"", it->file_path, "\"が重複登録されました。(ID:", it->full_id, ")");
			return false;
		}
		path_map_[it->file_path] = it;
		return true;
	}

	bool standard_resource_manager::load_resource_list() {
		auto file_path_list = file_accessor_->get_all_file_paths();
		boost::optional<std::string> tag_list_path;
		std::unordered_map<std::string, std::unique_ptr<resource_data>> full_id_to_resource_data_map;
		for (auto &&file_path : file_path_list) {
			//ファイル名本体を取得
			boost::filesystem::path path(file_path);
			auto file_name = path.filename().string();
			//ファイルがタグリストならスキップする
			if (file_name == tag_list_file_name) {
				tag_list_path = file_path;
				continue;
			}
			auto file_base_name = path.stem().string();
			auto meta_file_name = file_name + meta_data_file_suffix;
			//先頭3文字を接頭辞として取得
			auto type_prefix = file_base_name.substr(0, resource_file_name_suffix_length);
			//接頭辞から型を取得
			auto ti_it = resource_type_prefix_to_type_map_.find(type_prefix);
			if (ti_it == resource_type_prefix_to_type_map_.end()) {
				PE_LOG_WARNING("リソースの読み込みに失敗しました。指定された接頭辞のリソースタイプは存在しません。(type_prefix:", type_prefix,
				               ", file_path: ", file_path, ")");
				continue;
			}
			auto res_dat = std::make_unique<resource_data>(resource_data{ti_it->second});
			//接頭辞を除いたものをリソースIDとする
			res_dat->id = file_base_name.substr(resource_file_name_suffix_length, file_base_name.size() - 3);
			//ファイル名(タイプ接頭辞+リソースIDをFullIDとする)
			res_dat->full_id = file_base_name;
			res_dat->file_path = file_path;
			res_dat->metadata_file_path = (path.parent_path() / meta_file_name).string();
			res_dat->has_metadata = file_accessor_->check_file_exist(res_dat->metadata_file_path);
			full_id_to_resource_data_map.emplace(res_dat->full_id, std::move(res_dat));
		}
		PE_LOG_MESSAGE("リソース一覧を構築しました。(", full_id_to_resource_data_map.size(), "個のリソース定義)");
		//タグの読み込み
		bool is_tag_list_loaded = false;
		if (tag_list_path) {
			auto tag_list_file = file_accessor_->load_file(*tag_list_path);
			if (json_file tag_list; tag_list.load(*tag_list_file)) {
				try {
					for (auto tag_map = tag_list.get_root().get_with_exception<std::unordered_map<
						     std::string, std::vector<std::string>>>(); const auto &[tag, full_ids] : *tag_map) {
						for (auto &&full_id : full_ids) {
							try { full_id_to_resource_data_map[full_id]->tags.push_back(tag); } catch (const
								std::out_of_range &) {
								PE_LOG_WARNING("タグリストにおいて、タグ(", tag, ")に存在しないリソース(FullID:", full_id, "が関連付けられました。");
							}
						}
					}
					is_tag_list_loaded = true;
				} catch (const json_type_error &e) {
					PE_LOG_WARNING("タグリストのルートオブジェクト読み込みに失敗しました。(file_path: ", tag_list_path, ", error: ", e.what(),
					               ")");
				}
			} else { PE_LOG_WARNING("タグリストの読み込みに失敗しました。(file_path: ", tag_list_path, ")"); }
		}
		//リソースの登録
		for (auto &data : full_id_to_resource_data_map | std::views::values) {
			register_resource_data(std::move(*data));
		}
		if (is_tag_list_loaded) {
			PE_LOG_MESSAGE("タグリストが読み込まれました。(file_path:", tag_list_path, ", タグ数:", tag_map_.size(), ")");
		} else { PE_LOG_MESSAGE("タグリストは読み込まれませんでした。"); }
		return true;
	}

	void standard_resource_manager::unload_all_loaded_resources() {
		set_not_unload_tags_impl({});
		size_t unload{0};
		for (auto &res_dat : resource_data_list_) {
			if (res_dat.is_loaded) {
				unload_resource(res_dat);
				++unload;
			}
		}
		PE_LOG_MESSAGE("すべてのリソース(", unload, "個)をアンロードしました。");
	}

	bool standard_resource_manager::set_not_unload_tags_impl(const std::set<std::string> &tags) {
		//新たに指定されたタグを求める
		std::vector<std::string> new_set_tags;
		std::ranges::set_difference(tags, not_unload_tags_,
		                            std::back_inserter(new_set_tags));
		//指定から外れたタグを求める
		std::vector<std::string> new_unset_tags;
		std::ranges::set_difference(not_unload_tags_, tags,
		                            std::back_inserter(new_unset_tags));
		//新たなタグに属するリソースをアンロード対象外指定する
		for (auto &&tag : new_set_tags) {
			auto it = tag_map_.find(tag);
			if (it == tag_map_.end()) {
				PE_LOG_ERROR("存在しないタグ\"", tag, "\"が指定されました。");
				return false;
			}
			for (auto &&res_dat : it->second) { res_dat->not_unload = true; }
		}
		//指定から外れたタグに属するリソースをアンロード対象外指定から外す
		for (auto &&tag : new_unset_tags) {
			auto it = tag_map_.find(tag);
			if (it == tag_map_.end()) {
				PE_LOG_ERROR("存在しないタグ\"", tag, "\"が指定されました。");
				return false;
			}
			for (auto &&res_dat : it->second) { res_dat->not_unload = false; }
		}
		not_unload_tags_ = tags;
		return true;
	}

	std::shared_ptr<resource_base> standard_resource_manager::get_resource_by_full_id(
		const std::string &full_id, const bool is_valid_not_preload_warning) {
		const auto it = full_id_map_.find(full_id);
		if (it == full_id_map_.end()) {
			PE_LOG_WARNING("定義されていないリソースが要求されました。(FullID:", full_id, ")");
			return nullptr;
		}
		if (!it->second->is_loaded) {
			if (is_valid_not_preload_warning) {
				PE_LOG_WARNING("読み込まれていないリソースが要求されたため、読み込みを試みました。(FullID:", full_id, ", パス:", it->second->file_path,
				               ")");
			}
			return load_resource(*it->second);
		} else {
			assert(it->second->resource != nullptr);
			return it->second->resource;
		}
	}

	std::shared_ptr<resource_base> standard_resource_manager::get_resource_by_path(
		const std::string &path, const std::string &root_path, const bool is_valid_not_preload_warning) {
		//必要ならルートパスを連結
		std::string u_path = unify_path(root_path.empty() ? path : root_path + "\\" + path);
		const auto it = path_map_.find(u_path);
		if (it == path_map_.end()) {
			PE_LOG_WARNING("定義されていないリソースが要求されました。(パス:", u_path, ")");
			return nullptr;
		}
		if (!it->second->is_loaded) {
			if (is_valid_not_preload_warning) {
				PE_LOG_WARNING("読み込まれていないリソースが要求されたため、読み込みを試みました。(ID:", it->second->id, ", パス:", u_path, ")");
			}
			return load_resource(*it->second);
		}
		assert(it->second->resource != nullptr);
		return it->second->resource;
	}

	resource_manager_internal_accessor standard_resource_manager::create_internal_manager_accessor() {
		resource_manager_internal_accessor mgr_accessor{
			[this](const std::type_info &type, const std::string &id, const bool is_valid_not_preload_warning) {
				return get_resource_by_full_id(get_full_id_from_type_and_id(type, id), is_valid_not_preload_warning);
			},
			[this](const std::type_info &type, const std::string &path, const std::string &root_path,
			       const bool is_valid_not_preload_warning) {
				return get_resource_by_path(path, root_path, is_valid_not_preload_warning);
			}
		};
		return mgr_accessor;
	}

	standard_resource_manager::standard_resource_manager() = default;
	standard_resource_manager::~standard_resource_manager() = default;

	void standard_resource_manager::on_resource_type_added(const std::type_info &type, const std::string &type_name,
	                                                     const std::string &type_prefix,
	                                                     const resource_creator_type &creator) {
		resource_type_data_map_.emplace(type, resource_type_data{type, type_name, type_prefix, creator});
		resource_type_prefix_to_type_map_.emplace(type_prefix, type);
	}

	bool standard_resource_manager::prepare_resources(const std::vector<std::string> &need_tag_groups) {
		size_t new_loaded{0}, already{0};
		for (auto &&tag : need_tag_groups) {
			auto it = tag_map_.find(tag);
			if (it == tag_map_.end()) {
				PE_LOG_ERROR("存在しないタグ\"", tag, "\"が指定されました。");
				return false;
			}
			for (auto &&res_data_it : it->second) {
				if (res_data_it->is_loaded) { ++already; } else {
					if (load_resource(*res_data_it) == nullptr) {
						PE_LOG_ERROR("リソースの読み込みに失敗しました。(タグ:", tag, ", ID:", res_data_it->id, ", パス:",
						             res_data_it->file_path, ")");
						return false;
					}
					++new_loaded;
				}
			}
		}
		PE_LOG_MESSAGE("指定されたタググループに含まれるリソースのうち、", already, "個のリソースが既に読み込まれていて、", new_loaded,
		               "個のリソースが新たに読み込まれました。");
		return true;
	}

	bool standard_resource_manager::unload_unused_resources() {
		size_t unload{0};
		//未使用リソースがなくなるまでループ。(依存関係によっては一度の走査でアンロードしきれないため)
		while (true) {
			size_t new_unload{0};
			for (auto &res_dat : resource_data_list_) {
				//参照カウントが1だったら未使用とみなす。アンロード不可フラグが立っていたら無視
				if (res_dat.is_loaded && res_dat.resource.use_count() == 1 && !res_dat.not_unload) {
					unload_resource(res_dat);
					++unload;
					++new_unload;
				}
			}
			if (new_unload == 0) { break; }
		}
		PE_LOG_MESSAGE(unload, "個の未使用リソースがアンロードされました。");
		return true;
	}

	bool standard_resource_manager::is_ready() const { return true; }

	double standard_resource_manager::get_prepare_progress() const { return 1.0; }

	bool standard_resource_manager::initialize() {
		assert(file_accessor_ != nullptr);
		if (load_resource_list() == false) {
			PE_LOG_ERROR("初期化に失敗しました。リソースリストの取得に失敗しました。");
			return false;
		}
		return true;
	}

	void standard_resource_manager::finalize() { unload_all_loaded_resources(); }

	bool standard_resource_manager::set_not_unload_tags(const std::set<std::string> &tags) {
		return set_not_unload_tags_impl(tags);
	}

	std::shared_ptr<resource_base> standard_resource_manager::get_resource_by_type_and_id(
		const std::type_info &type, const std::string &id) {
		return get_resource_by_full_id(get_full_id_from_type_and_id(type, id), true);
	}

	void standard_resource_manager::set_file_manipulator(const std::shared_ptr<file_manipulator> &f_manipulator) {
		file_accessor_ = f_manipulator;
	}

	void standard_resource_manager::set_resource_list_file_name(const std::string &file_name) {
		resource_list_file_name_ = file_name;
	}

	std::string standard_resource_manager::get_full_id_from_type_and_id(const boost::typeindex::type_index &type_index,
	                                                                  const std::string &id) {
		const auto it = resource_type_data_map_.find(type_index.type_info());
		if (it == resource_type_data_map_.end()) { return "NULL"; }
		//IDにリソースタイプのPrefixを付けたものをフルIDとして返す
		return it->second.type_prefix + id;
	}

	std::shared_ptr<resource_base> standard_resource_manager::get_resource_by_full_id(const std::string &full_id) {
		return get_resource_by_full_id(full_id, true);
	}
}
