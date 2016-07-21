#include <cassert>

#include"ResourceManager.h"
#include "FileSystemManager.h"
#include "RCsv.h"
#include "SystemLog.h"
#include "FileAccessor.h"
#include "SystemVariables.h"

namespace planeta {
	namespace core {

		bool ResourceManager::PrepareResources(const std::vector<std::string>& need_tags) {
			auto ret = std::move(_check_newuse_and_nouse_tag_groups(need_tags));
			size_t new_num(ret.first.size()), no_num(ret.second.size()); //新規読み込み数と未使用数を記録
			//アンロードするべきタグは、未使用タググループリストに登録
			for (const std::string& tag : ret.second) {
				_AddUnusedtagGroups(tag);
			}
			auto load_tags = std::move(ret.first);
			//新規読み込み
			for (const auto& tag : load_tags) {
				_load_tag_group(tag);
			}
			PE_LOG_MESSAGE(new_num, "個のタググループが新規読み込みされ、", no_num, "個のタグが未使用となりました。");
			return true;
		}

		bool ResourceManager::UnloadUnusedResouces() {
			for (const auto& res_array : _unused_tag_map) {
				for (const auto& id : res_array.second) {
					id.second->Dispose();
				}
			}
			PE_LOG_MESSAGE(_unused_tag_map.size(), "個の未使用タググループがアンロードされました。");
			_unused_tag_map.clear();
			return true;
		}

		bool ResourceManager::IsReady()const { return true; }

		double ResourceManager::GetPrepairProgress() const { return 1.0; }

		std::shared_ptr<ResourceBase> ResourceManager::_CreateResource(const std::string& attribute, const std::shared_ptr<const File>& file) {
			auto it = _resource_creator_map.find(attribute);
			if (it == _resource_creator_map.end()) { return nullptr; }
			return (it->second)(file);
		}

		bool ResourceManager::_load_tag_group(const std::string& tag) {
			auto using_map_it = _using_tag_id_map.find(tag);
			//すでに読み込まれている
			if (using_map_it != _using_tag_id_map.end()) { return true; } else { //読み込まれていない
				_using_tag_id_map.emplace(tag, std::vector<std::string>());
				using_map_it = _using_tag_id_map.find(tag);
			}
			//未使用リストに存在する
			auto unused_map_it = _unused_tag_map.find(tag);
			if (unused_map_it != _unused_tag_map.end()) {
				for (const auto& res : unused_map_it->second) {
					using_map_it->second.push_back(res.first); //使用中リソースタグマップにIDを追加
					_using_resources.insert(res); //使用中リソースIDマップに追加
				}
				_unused_tag_map.erase(unused_map_it); //未使用リストから削除
			} else { //新規読み込み
				auto tag_map_it = _tag_resouce_map.find(tag);
				if (tag_map_it == _tag_resouce_map.end()) {
					PE_LOG_ERROR("要求されたタググループは存在しません。(", tag, ")");
					return false;
				}
				std::vector<std::string> ids;
				for (auto it = tag_map_it->second.begin(); it != tag_map_it->second.end(); ++it) {
					std::shared_ptr<const File> f = file_accessor_->LoadFile(it->file_name);
					if (f) {
						std::shared_ptr<ResourceBase> new_res = _CreateResource(it->type, f);
						if (new_res == nullptr) {
							PE_LOG_ERROR("リソースの作成に失敗しました。(タググループ=", tag, ",ID=", it->id, ",ファイル名=", it->file_name, ")");
						} else {
							_using_resources.emplace(it->id, new_res);
							ids.push_back(it->id);
						}
					} else {
						PE_LOG_ERROR("リソースファイルの読み込みに失敗しました。(タググループ\"", tag, "\"の\"", it->file_name, "\")");
					}
				}
				_using_tag_id_map.emplace(tag, std::move(ids));
			}
			return true;
		}

		bool ResourceManager::_LoadResourceList() {
			std::shared_ptr<const File> file = file_accessor_->LoadFile(_resource_list_file_name);
			if (file == nullptr) {
				PE_LOG_ERROR("リソースリスト(", _resource_list_file_name, ")の読み込みに失敗しました。");
				return false;
			}
			if (file->GetStatus() != File::FileStatus::Available) {
				PE_LOG_ERROR("リソースリストファイルを読み込めませんでした。(", _resource_list_file_name, ")", _tag_resouce_map.size(), +"個)");
				return false;
			}
			auto csv = MakeResource<RCsv>();
			if (csv->Create(file) == false) {
				PE_LOG_ERROR("リソースリストファイルをCSV形式として読み込めませんでした。(", _resource_list_file_name, ")", _tag_resouce_map.size(), "個)");
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
				rd.file_name = l[1];
				rd.type = l[2];
				for (unsigned int i = 3; i < l.size(); ++i) {
					auto it = _tag_resouce_map.find(l[i]);
					//タグが初登場だったら追加し、すでに登場していたらそれを指すイテレータを準備する
					if (it == _tag_resouce_map.end()) {
						_tag_resouce_map.emplace(l[i], std::vector<ResourceData>());
						it = _tag_resouce_map.find(l[i]);
					}
					it->second.push_back(rd);
				}
			}
			int total_resource_num = 0;
			for (const auto& tag_group : _tag_resouce_map) {
				total_resource_num += tag_group.second.size();
			}
			PE_LOG_MESSAGE("リソースリストを読み込みました。(", _tag_resouce_map.size(), "個のタググループ、合計", total_resource_num, "個のリソース定義)");
			return true;
		}

		bool ResourceManager::Initialize() {
			assert(file_accessor_ != nullptr);
			if (_LoadResourceList() == false) {
				PE_LOG_ERROR("初期化に失敗しました。リソースリストの取得に失敗しました。");
				return false;
			}
			return true;
		}

		void ResourceManager::Finalize() {
			_UnloadAllLoadedResources();
		}

		void ResourceManager::_UnloadAllLoadedResources() {
			for (auto& res : _using_resources) {
				res.second->Dispose();
			}
			for (auto& res : _unused_tag_map) {
				for (auto& p : res.second) {
					p.second->Dispose();
				}
			}
			_using_resources.clear();
			_using_tag_id_map.clear();
			_unused_tag_map.clear();
			PE_LOG_MESSAGE("すべてのリソースをアンロードしました。");
		}

		void ResourceManager::_AddUnusedtagGroups(const std::string& tag) {
			auto unused_map_it = _unused_tag_map.find(tag);
			if (unused_map_it == _unused_tag_map.end()) {
				_unused_tag_map.emplace(tag, std::vector<std::pair<std::string, std::shared_ptr<ResourceBase>>>());
				unused_map_it = _unused_tag_map.find(tag);
			} else { return; }
			auto it = _using_tag_id_map.find(tag);
			if (it != _using_tag_id_map.end()) {
				for (const auto& id : it->second) {
					auto rit = _using_resources.find(id);
					if (rit != _using_resources.end()) {
						unused_map_it->second.push_back(std::make_pair(id, rit->second));
						_using_resources.erase(rit);
					}
				}
				_using_tag_id_map.erase(it);
			}
		}

		std::shared_ptr<ResourceBase> ResourceManager::GetResource(const std::string& id) {
			auto it = _using_resources.find(id);
			if (it == _using_resources.end()) {
				PE_LOG_WARNING("読み込まれていないリソースが要求されました。(ID:", id, ")");
				return nullptr;
			} else {
				return it->second;
			}
		}

		const std::unordered_set<std::string> ResourceManager::GetUsingTagGroups() const {
			std::unordered_set<std::string> out;
			for (const auto& t : _using_tag_id_map) {
				out.insert(t.first);
			}
			return std::move(out);
		}

		std::pair<std::vector<std::string>, std::vector<std::string>> ResourceManager::_check_newuse_and_nouse_tag_groups(const std::vector<std::string>& need_tags) {
			std::vector<std::string> new_use, no_use;
			std::unordered_set<std::string> need_tags_set;
			//新たに読み込むべきタグを検索(ついでに未使用タグの検索を高速化するため、必要タグの連想配列を作っておく)
			for (const auto& tag : need_tags) {
				if (_using_tag_id_map.find(tag) == _using_tag_id_map.end()) { new_use.push_back(tag); }
				need_tags_set.insert(tag);
			}
			//未使用になるタグを検索
			for (const auto& tagp : _using_tag_id_map) {
				if (need_tags_set.find(tagp.first) == need_tags_set.end()) { no_use.push_back(tagp.first); }
			}
			return std::make_pair(new_use, no_use);
		}

		void ResourceManager::SetFileAccessor_(const std::shared_ptr<FileAccessor>& f_scsr) {
			file_accessor_ = f_scsr;
		}

	}
}