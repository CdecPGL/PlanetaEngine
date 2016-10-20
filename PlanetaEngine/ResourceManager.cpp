﻿#include <cassert>

#include"ResourceManager.h"
#include "FileSystemManager.h"
#include "RCsv.h"
#include "SystemLog.h"
#include "FileAccessor.h"
#include "SystemVariables.h"

namespace planeta {
	namespace private_ {
		//////////////////////////////////////////////////////////////////////////
		//Impl_
		//////////////////////////////////////////////////////////////////////////
		class ResourceManager::Impl_ {
		public:
			std::shared_ptr<FileAccessor> file_accessor_;
			/*リソースリストのファイル名*/
			std::string _resource_list_file_name;

			struct ResourceData {
				ResourceData() = default;
				ResourceData(const ResourceData&) = delete;
				ResourceData(ResourceData&&) = default;
				ResourceData& operator=(const ResourceData&) = delete;
				ResourceData& operator= (ResourceData&&) = default;
				/*ID*/
				std::string id = "\0";
				/*ファイルパス*/
				std::string file_path = "\0";
				/*種類*/
				std::string type = "\0";
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
			std::unordered_map<std::string, ResourceDataListType::iterator> id_map_;
			std::unordered_map<std::string, ResourceDataListType::iterator> path_map_;
			std::set<std::string> not_unload_tags_;

			
			/*ResourceのタイプによるResourceクリエータマップ*/
			std::unordered_map<std::string, _ResourceCreatorType> _resource_creator_map;
			/*リソースの作成*/
			std::shared_ptr<ResourceBase> _CreateResourceInstanceAndInitialize(const std::string& type, const std::shared_ptr<const File>& file);
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
		};

		std::shared_ptr<ResourceBase> ResourceManager::Impl_::_CreateResourceInstanceAndInitialize(const std::string& attribute, const std::shared_ptr<const File>& file) {
			auto it = _resource_creator_map.find(attribute);
			if (it == _resource_creator_map.end()) { return nullptr; }
			return (it->second)(file);
		}

		std::shared_ptr<planeta::private_::ResourceBase> ResourceManager::Impl_::LoadResource_(ResourceData& res_dat) {
			assert(res_dat.is_loaded == false);
			assert(res_dat.resouce == nullptr);
			auto file = file_accessor_->LoadFile(res_dat.file_path);
			if (file == nullptr) {
				PE_LOG_ERROR("リソースの読み込みに失敗しました。ファイルを読み込めませんでした。");
				return nullptr;
			}
			std::shared_ptr<ResourceBase> res;
			if (res_dat.resouce) {
				res = res_dat.resouce;
				if (res->Create(*file) == false) {
					res.reset();
				}
			} else {
				res = _CreateResourceInstanceAndInitialize(res_dat.type, file);
			}
			if (res == nullptr) {
				PE_LOG_ERROR("リソースの読み込みに失敗しました。リソースの作成でエラーが発生しました。");
				return nullptr;
			}
			res_dat.is_loaded = true;
			res_dat.resouce = res;
			return res;
		}

		void ResourceManager::Impl_::UnloadResource_(ResourceData& res_dat) {
			assert(res_dat.is_loaded);
			assert(res_dat.resouce != nullptr);
			res_dat.resouce->Dispose();
			res_dat.resouce.reset();
			res_dat.is_loaded = false;
		}

		bool ResourceManager::Impl_::RegisterResourceData_(ResourceData&& res_dat) {
			resource_data_list_.push_back(std::move(res_dat));
			auto it = --resource_data_list_.end();
			for (auto&& tag : it->tags) {
				tag_map_[tag].push_back(it);
			}
			auto id_map_it = id_map_.find(it->id);
			if (id_map_.find(it->id) != id_map_.end()) {
				PE_LOG_ERROR("ID\"", it->id, "\"が重複登録されました。(パス:", it->file_path, ")");
				return false;
			}
			id_map_[it->id] = it;
			if (path_map_.find(it->id) != path_map_.end()) {
				PE_LOG_ERROR("パス\"", it->file_path, "\"が重複登録されました。(ID:", it->id, ")");
				return false;
			}
			path_map_[it->file_path] = it;
			return true;
		}

		bool ResourceManager::Impl_::_LoadResourceList() {
			std::shared_ptr<const File> file = file_accessor_->LoadFile(_resource_list_file_name);
			if (file == nullptr) {
				PE_LOG_ERROR("リソースリスト(", _resource_list_file_name, ")の読み込みに失敗しました。");
				return false;
			}
			if (!file->is_available()) {
				PE_LOG_ERROR("リソースリストファイルを読み込めませんでした。(", _resource_list_file_name, ")");
				return false;
			}
			auto csv = MakeResource<RCsv>();
			if (csv->Create(*file) == false) {
				PE_LOG_ERROR("リソースリストファイルをCSV形式として読み込めませんでした。(", _resource_list_file_name, ")");
				return false;
			}
			for (const auto& l : *csv) {
				ResourceData rd;
				std::vector<std::string> tags;
				if (l.size() < 4) {
					std::string line;
					for (auto it = l.begin(); it != l.end(); ++it) {
						line += *it;
						if (it != l.end() - 1) { line += ","; }
					}
					PE_LOG_WARNING("リソース定義の項目数が足りません。この行はスキップします。(", line, ")");
					continue;
				}
				rd.id = l[0];
				rd.file_path = l[1];
				rd.type = l[2];
				for (unsigned int i = 3; i < l.size(); ++i) {
					rd.tags.push_back(l[i]);
				}
				if (!RegisterResourceData_(std::move(rd))) {
					PE_LOG_ERROR("リソース定義(ID:", rd.id, ", パス:", rd.file_path, ")が不正です。");
					return false;
				}
			}
			PE_LOG_MESSAGE("リソースリストを読み込みました。(", resource_data_list_.size(), "個のリソース定義)");
			return true;
		}

		void ResourceManager::Impl_::_UnloadAllLoadedResources() {
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

		bool ResourceManager::Impl_::SetNotUnloadTags_(const std::set<std::string>& tags) {
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
		//////////////////////////////////////////////////////////////////////////
		//ResourceManager
		//////////////////////////////////////////////////////////////////////////
		ResourceManager::ResourceManager():impl_(std::make_unique<Impl_>()) {}
		ResourceManager::~ResourceManager() = default;

		void ResourceManager::AddResourceCreatorMap_(const std::string& type_name, const _ResourceCreatorType& creator) {
			impl_->_resource_creator_map.emplace(type_name, creator);
		}

		bool ResourceManager::PrepareResources(const std::vector<std::string>& need_tags) {
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

		bool ResourceManager::UnloadUnusedResouces() {
			auto& resource_data_list_ = impl_->resource_data_list_;
			size_t unload{ 0 };
			for (auto& res_dat : resource_data_list_) {
				//参照カウントが1だったら未使用とみなす。アンロード不可フラグが立っていたら無視
				if (res_dat.is_loaded && res_dat.resouce.use_count() == 1 && !res_dat.not_unload) {
					impl_->UnloadResource_(res_dat);
					++unload;
				}
			}
			PE_LOG_MESSAGE(unload, "個の未使用リソースがアンロードされました。");
			return true;
		}

		bool ResourceManager::IsReady()const { return true; }

		double ResourceManager::GetPrepairProgress() const { return 1.0; }

		bool ResourceManager::Initialize() {
			assert(impl_->file_accessor_ != nullptr);
			if (impl_->_LoadResourceList() == false) {
				PE_LOG_ERROR("初期化に失敗しました。リソースリストの取得に失敗しました。");
				return false;
			}
			return true;
		}

		void ResourceManager::Finalize() {
			impl_->_UnloadAllLoadedResources();
		}

		bool ResourceManager::SetNotUnloadTags(const std::set<std::string>& tags) {
			return impl_->SetNotUnloadTags_(tags);
		}

		std::shared_ptr<ResourceBase> ResourceManager::GetResourceByID(const std::string& id) {
			auto& id_map_ = impl_->id_map_;
			auto it = id_map_.find(id);
			if (it == id_map_.end()) {
				PE_LOG_WARNING("定義されていないリソースが要求されました。(ID:", id, ")");
				return nullptr;
			}
			if (!it->second->is_loaded) {
				PE_LOG_WARNING("読み込まれていないリソースが要求されたため、読み込みを試みました。(ID:", id, ", パス:", it->second->file_path, ")");
				return impl_->LoadResource_(*it->second);
			} else {
				assert(it->second->resouce != nullptr);
				return it->second->resouce;
			}
		}

		std::shared_ptr<ResourceBase> ResourceManager::GetResourceByPath(const std::string& path) {
			auto& path_map_ = impl_->path_map_;
			auto it = path_map_.find(path);
			if (it == path_map_.end()) {
				PE_LOG_WARNING("定義されていないリソースが要求されました。(パス:", path, ")");
				return nullptr;
			}
			if (!it->second->is_loaded) {
				PE_LOG_WARNING("読み込まれていないリソースが要求されたため、読み込みを試みました。(ID:", it->second->id, ", パス:", path, ")");
				return impl_->LoadResource_(*it->second);
			} else {
				assert(it->second->resouce != nullptr);
				return it->second->resouce;
			}
		}

		void ResourceManager::SetFileAccessor_(const std::shared_ptr<FileAccessor>& f_scsr) {
			impl_->file_accessor_ = f_scsr;
		}

		void ResourceManager::SetResourceListFileName_(const std::string& file_name) {
			impl_->_resource_list_file_name = file_name;
		}

	}
}