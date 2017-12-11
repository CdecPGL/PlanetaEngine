#include <cassert>

#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"
#include "boost/type_index.hpp"

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
			constexpr char* ANONYMOUS_ID{"<ID未設定>"};
			constexpr char* META_DATA_FILE_SUFFIX{ "_meta.json" };
			constexpr unsigned int RESOURCE_FILE_NAME_SUFFIX_LENGTH{ 3 };
		}
		//////////////////////////////////////////////////////////////////////////
		//Impl_
		//////////////////////////////////////////////////////////////////////////
		class StandardResourceManager::Impl_ {
		public:
			std::shared_ptr<FileManipulator> file_accessor_;
			/*リソースリストのファイル名*/
			std::string _resource_list_file_name;

			struct ResourceData {
				/*ID*/
				std::string id = "";
				/*フルID*/
				std::string full_id = "";
				/*IDが割り当てられているか*/
				bool is_id_assigned = false;
				/*ファイルパス*/
				std::string file_path = "";
				/*メタデータファイルパス*/
				std::string metadata_file_path = "";
				/*種類*/
				boost::typeindex::type_index type_index;
				/*リソース本体*/
				std::shared_ptr<ResourceBase> resouce;
				/*リソースがロードされているか*/
				bool is_loaded = false;
				/*タグ*/
				std::vector<std::string> tags;
				/*アンロード対象外か*/
				bool not_unload = false;
			};
			using ResourceDataListType = std::list<ResourceData>;
			ResourceDataListType resource_data_list_;
			std::unordered_map<std::string, std::vector<ResourceDataListType::iterator>> tag_map_;
			std::unordered_map<std::string, ResourceDataListType::iterator> full_id_map_;
			std::unordered_map<std::string, ResourceDataListType::iterator> path_map_;
			std::set<std::string> not_unload_tags_;

			struct ResourceTypeData {
				const std::type_info& type;
				std::string type_name;
				std::string type_prefix;
				ResourceCreatorType creator;
			};
			/*ResourceのタイプによるResourceタイプデータ*/
			std::unordered_map<boost::typeindex::type_index, ResourceTypeData> resource_type_data_map_;
			/*リソースの作成*/
			std::shared_ptr<ResourceBase> CreateResource(const boost::typeindex::type_index& type);
			/*リソースのロード*/
			std::shared_ptr<ResourceBase> LoadResource_(ResourceData& res_dat);
			/*リソースのアンロード*/
			void UnloadResource_(ResourceData& res_dat);
			/*リソースデータの登録*/
			bool RegisterResourceData_(ResourceData&& res_dat);
			/*アンロードタグの指定*/
			bool SetNotUnloadTags_(const std::set<std::string>& tags);
			/*リソースリストの読み込み*/
			bool _LoadResourceList();
			/*読み込まれているすべてのリソースをアンロードする*/
			void _UnloadAllLoadedResources();
			/*内部マネージャアクセサを取得*/
			ResourceManagerInternalAccessor CreateInternalManagerAccessor();
			/*リソースをIDで取得する。ロードされていないリソース指定時に警告を出すか指定可能*/
			std::shared_ptr<ResourceBase> GetResourceByTypeAndID(const std::type_info& type, const std::string& id, bool is_valid_not_preload_warning);
			/*リソースをパスで取得する。ロードされていないリソース指定時に警告を出すか指定可能*/
			std::shared_ptr<ResourceBase> GetResourceByTypeAndPath(const std::type_info& type, const std::string& path, const std::string& root_path, bool is_valid_not_preload_warning);
		};

		std::shared_ptr<ResourceBase> StandardResourceManager::Impl_::CreateResource(const boost::typeindex::type_index& type) {
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

		std::shared_ptr<planeta::ResourceBase> StandardResourceManager::Impl_::LoadResource_(ResourceData& res_dat) {
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
			}else if (!metadata->Load(*metadata_file)) {
				PE_LOG_ERROR("メタデータファイルの読み込みに失敗しました。Jsonファイルとして読み込めませんでした。");
				return nullptr;
			}
			//マネージャアクセサ作成
			ResourceManagerInternalAccessor mgr_acsr{ CreateInternalManagerAccessor() };
			//インスタンス作成と初期化
			auto resource = CreateResource(res_dat.type_index);
			if (resource->Create(*file, *metadata, mgr_acsr) == false) {
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

		void StandardResourceManager::Impl_::UnloadResource_(ResourceData& res_dat) {
			assert(res_dat.is_loaded);
			assert(res_dat.resouce != nullptr);
			res_dat.resouce->Dispose();
			res_dat.resouce.reset();
			res_dat.is_loaded = false;
		}

		bool StandardResourceManager::Impl_::RegisterResourceData_(ResourceData&& res_dat) {
			resource_data_list_.push_back(std::move(res_dat));
			auto it = --resource_data_list_.end();
			//タグマップに登録
			for (auto&& tag : it->tags) {
				tag_map_[tag].push_back(it);
			}
			//フルIDマップに登録
			if (it->is_id_assigned) {
				auto full_id_map_it = full_id_map_.find(it->full_id);
				if (full_id_map_.find(it->full_id) != full_id_map_.end()) {
					PE_LOG_ERROR("FullID\"", it->full_id, "\"が重複登録されました。(パス:", it->file_path, ")");
					return false;
				}
				full_id_map_[it->full_id] = it;
			}
			//Pathマップに登録
			if (path_map_.find(it->full_id) != path_map_.end()) {
				PE_LOG_ERROR("パス\"", it->file_path, "\"が重複登録されました。(ID:", it->full_id, ")");
				return false;
			}
			path_map_[it->file_path] = it;
			return true;
		}

		bool StandardResourceManager::Impl_::_LoadResourceList() {
			auto file_path_list = file_accessor_->GetAllFilePaths();
			for (auto&& file_path : file_path_list) {
				//ファイル名本体を取得
				boost::filesystem::path path(file_path);
				auto file_name = path.filename().string();
				auto file_base_name = path.stem().string();
				auto meta_file_name = file_base_name + META_DATA_FILE_SUFFIX;
				//先頭3文字を接頭辞として取得
				auto type_prefix = file_base_name.substr(0, RESOURCE_FILE_NAME_SUFFIX_LENGTH);
				//接頭辞を除いたものをリソースIDとする
				auto resource_id = file_base_name.substr(RESOURCE_FILE_NAME_SUFFIX_LENGTH, file_base_name.size() - 3);

				ResourceData res_dat{};
				res_dat.id = resource_id;
				res_dat.full_id = file_base_name;
				res_dat.file_path = file_path;
				res_dat.metadata_file_path = (path.parent_path() / meta_file_name).string();
			}

			PE_LOG_MESSAGE("リソースリストを読み込みました。(", resource_data_list_.size(), "個のリソース定義)");
			return true;
		}

		void StandardResourceManager::Impl_::_UnloadAllLoadedResources() {
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

		bool StandardResourceManager::Impl_::SetNotUnloadTags_(const std::set<std::string>& tags) {
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

		std::shared_ptr<ResourceBase> StandardResourceManager::Impl_::GetResourceByTypeAndID(const std::type_info& type_info, const std::string& id, bool is_valid_not_preload_warning) {
			auto it = id_map_.find(id);
			if (it == id_map_.end()) {
				PE_LOG_WARNING("定義されていないリソースが要求されました。(ID:", id, ")");
				return nullptr;
			}
			if (!it->second->is_loaded) {
				if (is_valid_not_preload_warning) {
					PE_LOG_WARNING("読み込まれていないリソースが要求されたため、読み込みを試みました。(ID:", id, ", パス:", it->second->file_path, ")");
				}
				return LoadResource_(*it->second);
			} else {
				assert(it->second->resouce != nullptr);
				return it->second->resouce;
			}
		}

		std::shared_ptr<ResourceBase> StandardResourceManager::Impl_::GetResourceByTypeAndPath(const std::type_info& type, const std::string& path, const std::string& root_path, bool is_valid_not_preload_warning) {
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
			} else {
				assert(it->second->resouce != nullptr);
				return it->second->resouce;
			}
		}

		ResourceManagerInternalAccessor StandardResourceManager::Impl_::CreateInternalManagerAccessor() {
			namespace sp = std::placeholders;
			ResourceManagerInternalAccessor mgr_acsr{
				std::bind(&Impl_::GetResourceByTypeAndID, this, sp::_1, sp::_2, sp::_3),
				std::bind(&Impl_::GetResourceByTypeAndPath, this, sp::_1, sp::_2, sp::_3, sp::_4)
			};
			return std::move(mgr_acsr);
		}
		//////////////////////////////////////////////////////////////////////////
		//ResourceManager
		//////////////////////////////////////////////////////////////////////////
		StandardResourceManager::StandardResourceManager():impl_(std::make_unique<Impl_>()) {}
		StandardResourceManager::~StandardResourceManager() = default;

		void StandardResourceManager::AddResourceTypeProc(const std::type_info& type, const std::string& type_name, const std::string& type_prefix, const ResourceCreatorType& creator) {
			impl_->resource_type_data_map_.emplace(type_name, type, type_name, type_prefix, creator);
		}

		bool StandardResourceManager::PrepareResources(const std::vector<std::string>& need_tags) {
			auto& tag_map_ = impl_->tag_map_;
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
						if (impl_->LoadResource_(*res_data_it) == nullptr) {
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
			auto& resource_data_list_ = impl_->resource_data_list_;
			size_t unload{ 0 };
			//未使用リソースがなくなるまでループ。(依存関係によっては一度の走査でアンロードしきれないため)
			while (true) {
				size_t new_unload{ 0 };
				for (auto& res_dat : resource_data_list_) {
					//参照カウントが1だったら未使用とみなす。アンロード不可フラグが立っていたら無視
					if (res_dat.is_loaded && res_dat.resouce.use_count() == 1 && !res_dat.not_unload) {
						impl_->UnloadResource_(res_dat);
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
			assert(impl_->file_accessor_ != nullptr);
			if (impl_->_LoadResourceList() == false) {
				PE_LOG_ERROR("初期化に失敗しました。リソースリストの取得に失敗しました。");
				return false;
			}
			return true;
		}

		void StandardResourceManager::Finalize() {
			impl_->_UnloadAllLoadedResources();
		}

		bool StandardResourceManager::SetNotUnloadTags(const std::set<std::string>& tags) {
			return impl_->SetNotUnloadTags_(tags);
		}

		std::shared_ptr<ResourceBase> StandardResourceManager::GetResourceByTypeAndID(const std::type_info& type, const std::string& id) {
			return impl_->GetResourceByTypeAndID(type, id, true);
		}

		void StandardResourceManager::SetFileManipulator_(const std::shared_ptr<FileManipulator>& f_scsr) {
			impl_->file_accessor_ = f_scsr;
		}

		void StandardResourceManager::SetResourceListFileName_(const std::string& file_name) {
			impl_->_resource_list_file_name = file_name;
		}

	}
}