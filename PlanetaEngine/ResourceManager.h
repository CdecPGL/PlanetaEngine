#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <thread>
#include <memory>
#include "Object.h"
#include "PointerSingletonTemplate.h"
#include "MakeResource.h"

namespace planeta_engine{
	namespace file_system {
		class FileAccessor;
		class File;
	}
	namespace core{
		class ResourceBase;
		/*Resource管理クラス。非同期読み込みは未実装だが、実装時に変更が必要ないように非同期読み込みと同じように扱う。
		PrepairResourcesでリソースの準備を開始し、IsReadyがtrueになるまで待つ。そしてUnloadUnusedResourscesを呼ぶ。
		現状ではIsReadyは常にtrue、GetPrepairProgressは常に1.0を返す。
		*/
		class ResourceManager : public utility::PointerSingletonTemplate<ResourceManager>{
			friend utility::PointerSingletonTemplate<ResourceManager>;
		public:
			bool Initialize()override;
			bool Finalize()override;
			/*Resourceの準備。読み込んでいないタググループを読み込み、読み込み済みでいらないものを破棄リストに登録する(param:必要なタグリスト)*/
			bool PrepareResources(const std::vector<std::string>& need_tag_groups);
			/*未使用のタググループをアンロードする*/
			bool UnloadUnusedResouces();
			/*Resourceの準備が完了したか*/
			bool IsReady()const;
			/*準備進行度(読み込みしていない時は1.0とする)*/
			double GetPrepairProgress()const;
			/*使っているタググループ名を取得*/
			const std::unordered_set<std::string> GetUsingTagGroups()const;
			/*リソースの属性を追加*/
			template<class C>
			void AddResourceType(const std::string& type_name) {
				_resource_creator_map.emplace(type_name, [](const std::shared_ptr<const file_system::File>& file)->std::shared_ptr<ResourceBase>{
					std::shared_ptr<ResourceBase> new_res = MakeResource<C>();
					return new_res->Create(file) ? new_res : nullptr;
				});
			}
			/*リソースリストファイル名を設定。初期化前に呼び出す必要がある*/
			void SetResourceListFileName(const std::string& file_name) { _resource_list_file_name = file_name; }
			/*リソースを取得*/
			std::shared_ptr<ResourceBase> GetResource(const std::string& id);
		private:
			ResourceManager()=default;
			ResourceManager(const ResourceManager&) = delete;
			~ResourceManager() {};
			std::shared_ptr<file_system::FileAccessor> file_accessor_;
			/*リソースリストのファイル名*/
			std::string _resource_list_file_name;

			struct ResourceData{
				/*ID*/
				std::string id = "\0";
				/*ファイル名*/
				std::string file_name = "\0";
				/*種類*/
				std::string type = "\0";
			};
			/*タグによるResourceデータマップ*/
			std::unordered_map<std::string, std::vector<ResourceData>> _tag_resouce_map;

			/*使われているリソースのタグマップ(タグ,ID配列)*/
			std::unordered_map<std::string, std::vector<std::string>> _using_tag_id_map;
			/*使われていないリソースのマップ<タグ,<ID,リソース>配列>*/
			std::unordered_map<std::string,std::vector<std::pair<std::string,std::shared_ptr<ResourceBase>>>> _unused_tag_map;
			/*使われているリソース(ID,リソース)*/
			std::unordered_map<std::string, std::shared_ptr<ResourceBase>> _using_resources;

			/*指定タググループを未使用タググループに追加*/
			void _AddUnusedtagGroups(const std::string& tag);
			/*タググループを読み込み*/
			bool _load_tag_group(const std::string& tag);
			/*必要なタグと現在読み込まれているタググループを比較し、新たに使うもの、もう使わないものを返す*/
			std::pair<std::vector<std::string>, std::vector<std::string>> _check_newuse_and_nouse_tag_groups(const std::vector<std::string>& need_tags);


			/*リソースクリエータ関数型*/
			using _ResourceCreatorType = std::function<std::shared_ptr<ResourceBase>(const std::shared_ptr<const file_system::File>&)>;
			/*ResourceのタイプによるResourceクリエータマップ*/
			std::unordered_map<std::string, _ResourceCreatorType> _resource_creator_map;
			/*リソースの作成*/
			std::shared_ptr<ResourceBase> _CreateResource(const std::string& type, const std::shared_ptr<const file_system::File>& file);

			/*リソースリストの読み込み*/
			bool _LoadResourceList();
			/*読み込まれているすべてのリソースをアンロードする*/
			void _UnloadAllLoadedResources();
		};
	}
}