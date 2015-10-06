#include"ResourceManager.h"
#include "boost/lexical_cast.hpp"
#include "FileLoadManager.h"
#include "CSVResource.h"
#include "SystemLog.h"

namespace planeta_engine{
	namespace core{

		bool ResourceManager::PrepareResources(const std::vector<std::string>& need_tags)
		{
			auto ret = std::move(_check_newuse_and_nouse_tag_groups(need_tags));
			size_t new_num(ret.first.size()), no_num(ret.second.size()); //新規読み込み数と未使用数を記録
			//アンロードするべきタグは、未使用タググループリストに登録
			for (const std::string& tag : ret.second) {
				_AddUnusedtagGroups(tag);
			}
			auto load_tags = std::move(ret.first);
			//新規読み込み
			for (const auto& tag : load_tags){
				_load_tag_group(tag);
			}
			debug::SystemLog::instance().LogMessage(boost::lexical_cast<std::string>(new_num) + "個のタググループが新規読み込みされ、" + boost::lexical_cast<std::string>(no_num) + "個のタグが未使用となりました。", "ResourceManager::PrepairResource");
			return true;
		}

		bool ResourceManager::UnloadUnusedResouces()
		{
			for (const auto& res_array : _unused_tag_map){
				for (const auto& id : res_array.second) {
					id.second->Dispose();
				}
			}
			debug::SystemLog::instance().LogMessage(boost::lexical_cast<std::string>(_unused_tag_map.size()) + "個の未使用タググループがアンロードされました。", "ResourceManager::UnloadUnusedResources");
			_unused_tag_map.clear();
			return true;
		}

		bool ResourceManager::IsReady()const{ return true; }

		double ResourceManager::GetPrepairProgress() const { return 1.0; }

		std::shared_ptr<ResourceBase> ResourceManager::_CreateResource(const std::string& attribute,const std::shared_ptr<file_system::File>& file)
		{
			auto it = _resource_creator_map.find(attribute);
			if (it == _resource_creator_map.end()) { return nullptr; }
			return (it->second)(file);
		}

		bool ResourceManager::_load_tag_group(const std::string& tag)
		{
			debug::SystemLog& sl = debug::SystemLog::instance();
			auto using_map_it = _using_tag_id_map.find(tag);
			//すでに読み込まれている
			if (using_map_it != _using_tag_id_map.end()) { return true;}
			else { //読み込まれていない
				_using_tag_id_map.emplace(tag, std::vector<std::string>());
				auto using_map_it = _using_tag_id_map.find(tag);
			}
			//未使用リストに存在する
			auto it = _unused_tag_map.find(tag);
			if (it != _unused_tag_map.end()) {
				for (const auto& res : it->second) {
					using_map_it->second.push_back(res.first); //使用中リソースタグマップにIDを追加
					_using_resources.insert(res); //使用中リソースIDマップに追加
				}
				_unused_tag_map.erase(it); //未使用リストから削除
			}
			else { //新規読み込み
				using namespace file_system;
				auto tag_map_it = _tag_resouce_map.find(tag);
				if (tag_map_it == _tag_resouce_map.end()) {
					debug::SystemLog::instance().LogError(std::string("要求されたタググループは存在しません。(") + tag + ")", "ResourceManager::_load_tag_group");
					return false;
				}
				std::vector<std::string> ids;
				for (auto it = tag_map_it->second.begin(); it != tag_map_it->second.end(); ++it) {
					std::shared_ptr<File> f = FileLoadManager::instance().LoadFile(it->file_name);
					if (f) {
						std::shared_ptr<ResourceBase> new_res = _CreateResource(it->type, f);
						if (new_res == nullptr) {
							debug::SystemLog::instance().LogError(std::string("リソースの作成に失敗しました。(") + "タググループ\"" + tag + "\"の\"" + it->file_name + "\")", "ResourceManager::_load_tag_group");
						}
						else {
							_using_resources.emplace(it->id, new_res);
							ids.push_back(it->id);
						}
					}
					else {
						debug::SystemLog::instance().LogError(std::string("リソースファイルの読み込みに失敗しました。(") + "タググループ\"" + tag + "\"の\"" + it->file_name + "\")", "ResourceManager::_load_tag_group");
					}
				}
				_using_tag_id_map.emplace(tag, std::move(ids));
			}
			return true;
		}

		bool ResourceManager::_LoadResourceList()
		{
			std::shared_ptr<file_system::File> file = file_system::FileLoadManager::instance().LoadFile(_resource_list_file_name);
			if (file == nullptr) {
				debug::SystemLog::instance().LogError(std::string("リソースリスト(") + _resource_list_file_name + ")の読み込みに失敗しました。","ResourceManager::_LoadResourceList");
				return false; 
			}
			if (file->GetStatus() != file_system::File::FileStatus::Available) {
				debug::SystemLog::instance().LogError(std::string("リソースリストファイルを読み込めませんでした。(" + _resource_list_file_name + ")") + boost::lexical_cast<std::string>(_tag_resouce_map.size()) + "個)", "ResourceManager::_LoadResourceList");
				return false;
			}
			auto csv = MakeResource<resources::CSVResource>();
			if (csv->Create(file) == false) {
				return false; 
				debug::SystemLog::instance().LogError(std::string("リソースリストファイルをCSV形式として読み込めませんでした。(" + _resource_list_file_name + ")") + boost::lexical_cast<std::string>(_tag_resouce_map.size()) + "個)", "ResourceManager::_LoadResourceList");
			}
			for (const auto& l : *csv) {
				ResourceData rd;
				std::vector<std::string> tags;
				if (l.size() < 4) { 
					std::string line;
					for (auto it = l.begin(); it != l.end();++it) {
						line += *it;
						if (it != l.end() - 1) { line += ","; }
					}
					debug::SystemLog::instance().LogWarning(std::string("リソース定義の項目数が足りません。この行はスキップします。(") + line + ")", "ResourceManager::_LoadResourceList");
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
			debug::SystemLog::instance().LogMessage(std::string("リソースリストを読み込みました。(") + boost::lexical_cast<std::string>(_tag_resouce_map.size()) + "個のタググループ、合計" + boost::lexical_cast<std::string>(total_resource_num) + "個のリソース定義)", "ResourceManager::_LoadResourceList");
			return true;
		}

		bool ResourceManager::Initialize()
		{
			if (_LoadResourceList() == false) {
				debug::SystemLog::instance().LogError("初期化に失敗しました。", "ResourceManagerInitialize");
				return false; 
			}
			debug::SystemLog::instance().LogMessage("初期化しました。", "ResourceManager::Initialize");
			return true;
		}

		bool ResourceManager::Finalize()
		{
			_UnloadAllLoadedResources();
			debug::SystemLog::instance().LogMessage("終了処理を行いました。", "ResourceManager::Finalize");
			return true;
		}

		void ResourceManager::_UnloadAllLoadedResources()
		{
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
			debug::SystemLog::instance().LogMessage("すべてのリソースをアンロードしました。", "ResourceManager::_UnloadAllLoadedResources");
		}

		void ResourceManager::_AddUnusedtagGroups(const std::string& tag)
		{
			auto unused_map_it = _unused_tag_map.find(tag);
			if (unused_map_it == _unused_tag_map.end()) {
				_unused_tag_map.emplace(tag, std::vector<std::pair<std::string, std::shared_ptr<ResourceBase>>>());
				unused_map_it = _unused_tag_map.find(tag);
			}
			else { return; }
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

		std::shared_ptr<ResourceBase> ResourceManager::GetResource(const std::string& id)
		{
			auto it = _using_resources.find(id);
			if (it == _using_resources.end()) {
				debug::SystemLog::instance().LogWarning(std::string("読み込まれていないリソースが要求されました。(ID:") + id + ")", "ResourceManager::GetResource");
				return nullptr;
			}
			else {
				return it->second;
			}
		}

		const std::unordered_set<std::string> ResourceManager::GetUsingTagGroups() const
		{
			std::unordered_set<std::string> out;
			for (const auto& t : _using_tag_id_map) {
				out.insert(t.first);
			}
			return std::move(out);
		}

		std::pair<std::vector<std::string>, std::vector<std::string>> ResourceManager::_check_newuse_and_nouse_tag_groups(const std::vector<std::string>& need_tags)
		{
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

	}
}