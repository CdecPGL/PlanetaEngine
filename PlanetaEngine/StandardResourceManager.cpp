#include <cassert>
//#include <optional>

#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"
#include "boost/optional.hpp"

#include"StandardResourceManager.h"
#include "File.h"
#include "JsonFile.h"
#include "LogUtility.h"
#include "FileManipulator.h"
#include "SystemVariables.h"
#include "ResourceReferencer.h"
#include "ResourceBase.h"

namespace {
	std::string UnifyPath(const std::string& src) {
		return boost::algorithm::replace_all_copy(src, "/", "\\");
	}
}

namespace planeta {
	namespace private_ {
		namespace {
			constexpr char* ANONYMOUS_ID{ "<ID未設定>" };
			constexpr char* META_DATA_FILE_SUFFIX{ ".meta.json" };
			constexpr unsigned int RESOURCE_FILE_NAME_SUFFIX_LENGTH{ 3 };
			constexpr char* TAG_LIST_FILE_NAME{ "tag_list.json" };
		}

		std::shared_ptr<ResourceBase> StandardResourceManager::CreateResource_(const std::type_index& type) {
			auto it = resource_type_data_map_.find(type);
			if (it == resource_type_data_map_.end()) { return nullptr; }
			//指定タイプのリソース作成
			auto res = it->second.creator();
			if (res == nullptr) {
				PE_LOG_ERROR("リソースのインスタンス作成に失敗しました。");
				return nullptr;
			}
			return res;
		}

		std::shared_ptr<planeta::ResourceBase> StandardResourceManager::LoadResource_(ResourceData_& res_dat) {
			assert(res_dat.is_loaded == false);
			assert(res_dat.resouce == nullptr);
			//リソースファイルを読み込み
			auto file = file_accessor_->LoadFile(res_dat.file_path);
			if (file == nullptr) {
				PE_LOG_ERROR("リソースの読み込みに失敗しました。ファイルを読み込めませんでした。");
				return nullptr;
			}
			//メタデータを読み込み
			std::unique_ptr<JsonFile> metadata = std::make_unique<JsonFile>();
			auto metadata_file = file_accessor_->LoadFile(res_dat.metadata_file_path);
			if (metadata_file == nullptr) {
				PE_LOG_MESSAGE(res_dat.file_path, "のメタデータファイルは存在しません。");
			}
			else if (!metadata->Load(*metadata_file)) {
				PE_LOG_ERROR("メタデータファイルの読み込みに失敗しました。Jsonファイルとして読み込めませんでした。");
				return nullptr;
			}
			//マネージャアクセサ作成
			ResourceManagerInternalAccessor mgr_acsr{ CreateInternalManagerAccessor_() };
			//インスタンス作成と初期化
			auto resource = CreateResource_(res_dat.type_index);
			if (resource->Load(*file, *metadata, mgr_acsr) == false) {
				resource.reset();
			}
			if (resource == nullptr) {
				PE_LOG_ERROR("リソースの読み込みに失敗しました。リソースの作成でエラーが発生しました。");
				return nullptr;
			}
			res_dat.is_loaded = true;
			res_dat.resouce = resource;
			return resource;
		}

		void StandardResourceManager::UnloadResource_(ResourceData_& res_dat) {
			assert(res_dat.is_loaded);
			assert(res_dat.resouce != nullptr);
			res_dat.resouce->Dispose();
			res_dat.resouce.reset();
			res_dat.is_loaded = false;
		}

		bool StandardResourceManager::RegisterResourceData_(ResourceData_&& res_dat) {
			resource_data_list_.push_back(std::move(res_dat));
			auto it = --resource_data_list_.end();
			//タグマップに登録
			for (auto&& tag : it->tags) {
				tag_map_[tag].push_back(it);
			}
			//フルIDマップに登録
			auto full_id_map_it = full_id_map_.find(it->full_id);
			if (full_id_map_.find(it->full_id) != full_id_map_.end()) {
				PE_LOG_ERROR("FullID\"", it->full_id, "\"が重複登録されました。(パス:", it->file_path, ")");
				return false;
			}
			full_id_map_[it->full_id] = it;
			//Pathマップに登録
			if (path_map_.find(it->full_id) != path_map_.end()) {
				PE_LOG_ERROR("パス\"", it->file_path, "\"が重複登録されました。(ID:", it->full_id, ")");
				return false;
			}
			path_map_[it->file_path] = it;
			return true;
		}

		bool StandardResourceManager::LoadResourceList_() {
			auto file_path_list = file_accessor_->GetAllFilePaths();
			boost::optional<std::string> tag_list_path;
			std::unordered_map<std::string, std::unique_ptr<ResourceData_>> full_id_to_resource_data_map;
			for (auto&& file_path : file_path_list) {
				//ファイル名本体を取得
				boost::filesystem::path path(file_path);
				auto file_name = path.filename().string();
				//ファイルがタグリストならスキップする
				if (file_name == TAG_LIST_FILE_NAME) {
					tag_list_path = file_path;
					continue;
				}
				auto file_base_name = path.stem().string();
				auto meta_file_name = file_name + META_DATA_FILE_SUFFIX;
				//先頭3文字を接頭辞として取得
				auto type_prefix = file_base_name.substr(0, RESOURCE_FILE_NAME_SUFFIX_LENGTH);
				//接頭辞から型を取得
				auto ti_it = resource_type_prefix_to_type_map_.find(type_prefix);
				if (ti_it == resource_type_prefix_to_type_map_.end()) {
					PE_LOG_WARNING("リソースの読み込みに失敗しました。指定された接頭辞のリソースタイプは存在しません。(type_prefix:", type_prefix, "file_path: ", file_path, ")");
					continue;
				}
				auto res_dat = std::make_unique<ResourceData_>(ResourceData_{ ti_it->second });
				//接頭辞を除いたものをリソースIDとする
				res_dat->id = file_base_name.substr(RESOURCE_FILE_NAME_SUFFIX_LENGTH, file_base_name.size() - 3);
				//ファイル名(タイプ接頭辞+リソースIDをFullIDとする)
				res_dat->full_id = file_base_name;
				res_dat->file_path = file_path;
				res_dat->metadata_file_path = (path.parent_path() / meta_file_name).string();
				full_id_to_resource_data_map.emplace(res_dat->full_id, std::move(res_dat));
			}
			PE_LOG_MESSAGE("リソース一覧を構築しました。(", resource_data_list_.size(), "個のリソース定義)");
			//タグの読み込み
			bool is_tag_list_loaded = false;
			if (tag_list_path) {
				auto tag_list_file = file_accessor_->LoadFile(*tag_list_path);
				JsonFile tag_list;
				if (tag_list.Load(*tag_list_file)) {
					try {
						auto tag_map = tag_list.GetRoot().GetWithException<std::unordered_map<std::string, std::vector<std::string>>>();
						for (auto&& pair : *tag_map) {
							auto tag = pair.first;
							for (auto&& full_id : pair.second) {
								try {
									full_id_to_resource_data_map[full_id]->tags.push_back(tag);
								}
								catch (const std::out_of_range&) {
									PE_LOG_WARNING("タグリストにおいて、タグ(", tag, ")に存在しないリソース(FullID:", full_id, "が関連付けられました。");
									continue;
								}
							}
						}
						is_tag_list_loaded = true;
					}
					catch (const JSONTypeError& e) {
						PE_LOG_WARNING("タグリストのルートオブジェクト読み込みに失敗しました。(file_path: ", tag_list_path, ", error: ", e.what(), ")");
					}
				}
				else {
					PE_LOG_WARNING("タグリストの読み込みに失敗しました。(file_path: ", tag_list_path, ")");
				}
			}
			//リソースの登録
			for (auto&& pair : full_id_to_resource_data_map) {
				RegisterResourceData_(std::move(*pair.second));
			}
			if (is_tag_list_loaded) {
				PE_LOG_MESSAGE("タグリストが読み込まれました。(file_path:", tag_list_path, ", タグ数:", tag_map_.size(), ")");
			}
			else {
				PE_LOG_MESSAGE("タグリストは読み込まれませんでした。");
			}
			return true;
		}

		void StandardResourceManager::UnloadAllLoadedResources_() {
			SetNotUnloadTags_({});
			size_t unload{ 0 };
			for (auto& res_dat : resource_data_list_) {
				if (res_dat.is_loaded) {
					UnloadResource_(res_dat);
					++unload;
				}
			}
			PE_LOG_MESSAGE("すべてのリソース(", unload, "個)をアンロードしました。");
		}

		bool StandardResourceManager::SetNotUnloadTags_(const std::set<std::string>& tags) {
			//新たに指定されたタグを求める
			std::vector<std::string> new_set_tags;
			std::set_difference(tags.begin(), tags.end(), not_unload_tags_.begin(), not_unload_tags_.end(), std::back_inserter(new_set_tags));
			//指定から外れたタグを求める
			std::vector<std::string> new_unset_tags;
			std::set_difference(not_unload_tags_.begin(), not_unload_tags_.end(), tags.begin(), tags.end(), std::back_inserter(new_unset_tags));
			//新たなタグに属するリソースをアンロード対象外指定する
			for (auto&& tag : new_set_tags) {
				auto it = tag_map_.find(tag);
				if (it == tag_map_.end()) {
					PE_LOG_ERROR("存在しないタグ\"", tag, "\"が指定されました。");
					return false;
				}
				for (auto&& res_dat : it->second) {
					res_dat->not_unload = true;
				}
			}
			//指定から外れたタグに属するリソースをアンロード対象外指定から外す
			for (auto&& tag : new_unset_tags) {
				auto it = tag_map_.find(tag);
				if (it == tag_map_.end()) {
					PE_LOG_ERROR("存在しないタグ\"", tag, "\"が指定されました。");
					return false;
				}
				for (auto&& res_dat : it->second) {
					res_dat->not_unload = false;
				}
			}
			not_unload_tags_ = tags;
			return true;
		}

		std::shared_ptr<ResourceBase> StandardResourceManager::GetResourceByFullID_(const std::string& full_id, bool is_valid_not_preload_warning) {
			auto it = full_id_map_.find(full_id);
			if (it == full_id_map_.end()) {
				PE_LOG_WARNING("定義されていないリソースが要求されました。(FullID:", full_id, ")");
				return nullptr;
			}
			if (!it->second->is_loaded) {
				if (is_valid_not_preload_warning) {
					PE_LOG_WARNING("読み込まれていないリソースが要求されたため、読み込みを試みました。(FullID:", full_id, ", パス:", it->second->file_path, ")");
				}
				return LoadResource_(*it->second);
			}
			else {
				assert(it->second->resouce != nullptr);
				return it->second->resouce;
			}
		}

		std::shared_ptr<ResourceBase> StandardResourceManager::GetResourceByPath_(const std::string& path, const std::string& root_path, bool is_valid_not_preload_warning) {
			//必要ならルートパスを連結
			std::string upath = UnifyPath(root_path.empty() ? path : root_path + "\\" + path);
			auto it = path_map_.find(upath);
			if (it == path_map_.end()) {
				PE_LOG_WARNING("定義されていないリソースが要求されました。(パス:", upath, ")");
				return nullptr;
			}
			if (!it->second->is_loaded) {
				if (is_valid_not_preload_warning) {
					PE_LOG_WARNING("読み込まれていないリソースが要求されたため、読み込みを試みました。(ID:", it->second->id, ", パス:", upath, ")");
				}
				return LoadResource_(*it->second);
			}
			else {
				assert(it->second->resouce != nullptr);
				return it->second->resouce;
			}
		}

		ResourceManagerInternalAccessor StandardResourceManager::CreateInternalManagerAccessor_() {
			namespace sp = std::placeholders;
			ResourceManagerInternalAccessor mgr_acsr{
				[this](const std::type_info& type, const std::string& id, bool is_valid_not_preload_warning) {
				return GetResourceByFullID_(GetFullIDFromTypeAndID_(type, id),is_valid_not_preload_warning);
			},
				[this](const std::type_info& type, const std::string& path, const std::string& root_path, bool is_valid_not_preload_warning) {
				return GetResourceByPath_(path, root_path, is_valid_not_preload_warning);
			}
			};
			return std::move(mgr_acsr);
		}

		StandardResourceManager::StandardResourceManager() = default;
		StandardResourceManager::~StandardResourceManager() = default;

		void StandardResourceManager::OnResourceTypeAdded(const std::type_info& type, const std::string& type_name, const std::string& type_prefix, const ResourceCreatorType& creator) {
			resource_type_data_map_.emplace(type, ResourceTypeData_{ type, type_name, type_prefix, creator });
			resource_type_prefix_to_type_map_.emplace(type_prefix, type);
		}

		bool StandardResourceManager::PrepareResources(const std::vector<std::string>& need_tags) {
			size_t new_loaded{ 0 }, already{ 0 };
			for (auto&& tag : need_tags) {
				auto it = tag_map_.find(tag);
				if (it == tag_map_.end()) {
					PE_LOG_ERROR("存在しないタグ\"", tag, "\"が指定されました。");
					return false;
				}
				for (auto&& res_data_it : it->second) {
					if (res_data_it->is_loaded) { ++already; }
					else {
						if (LoadResource_(*res_data_it) == nullptr) {
							PE_LOG_ERROR("リソースの読み込みに失敗しました。(タグ:", tag, ", ID:", res_data_it->id, ", パス:", res_data_it->file_path, ")");
							return false;
						}
						++new_loaded;
					}
				}
			}
			PE_LOG_MESSAGE("指定されたタググループに含まれるリソースのうち、", already, "個のリソースが既に読み込まれていて、", new_loaded, "個のリソースが新たに読み込まれました。");
			return true;
		}

		bool StandardResourceManager::UnloadUnusedResouces() {
			size_t unload{ 0 };
			//未使用リソースがなくなるまでループ。(依存関係によっては一度の走査でアンロードしきれないため)
			while (true) {
				size_t new_unload{ 0 };
				for (auto& res_dat : resource_data_list_) {
					//参照カウントが1だったら未使用とみなす。アンロード不可フラグが立っていたら無視
					if (res_dat.is_loaded && res_dat.resouce.use_count() == 1 && !res_dat.not_unload) {
						UnloadResource_(res_dat);
						++unload;
						++new_unload;
					}
				}
				if (new_unload == 0) { break; }
			}
			PE_LOG_MESSAGE(unload, "個の未使用リソースがアンロードされました。");
			return true;
		}

		bool StandardResourceManager::IsReady()const { return true; }

		double StandardResourceManager::GetPrepairProgress() const { return 1.0; }

		bool StandardResourceManager::Initialize() {
			assert(file_accessor_ != nullptr);
			if (LoadResourceList_() == false) {
				PE_LOG_ERROR("初期化に失敗しました。リソースリストの取得に失敗しました。");
				return false;
			}
			return true;
		}

		void StandardResourceManager::Finalize() {
			UnloadAllLoadedResources_();
		}

		bool StandardResourceManager::SetNotUnloadTags(const std::set<std::string>& tags) {
			return SetNotUnloadTags_(tags);
		}

		std::shared_ptr<ResourceBase> StandardResourceManager::GetResourceByTypeAndID(const std::type_info& type, const std::string& id) {
			return GetResourceByFullID_(GetFullIDFromTypeAndID_(type, id), true);
		}

		void StandardResourceManager::SetFileManipulator_(const std::shared_ptr<FileManipulator>& f_scsr) {
			file_accessor_ = f_scsr;
		}

		void StandardResourceManager::SetResourceListFileName_(const std::string& file_name) {
			resource_list_file_name_ = file_name;
		}

		std::string StandardResourceManager::GetFullIDFromTypeAndID_(const boost::typeindex::type_index& type_index, const std::string& id) {
			auto it = resource_type_data_map_.find(type_index.type_info());
			if (it == resource_type_data_map_.end()) { return "NULL"; }
			//IDにリソースタイプのPrefixを付けたものをフルIDとして返す
			return it->second.type_prefix + id;
		}

		std::shared_ptr<planeta::ResourceBase> StandardResourceManager::GetResourceByFullID(const std::string& full_id) {
			return GetResourceByFullID_(full_id, true);
		}

	}
}