#pragma once

#include <set>
#include <vector>
#include <functional>
#include "IResourceManager.h"
#include "SubSystemManager.h"

namespace planeta {
	class FileManipulator;
	namespace private_ {
		/*リソースマネージャ*/
		class ResourceManager : public IResourceManager, public SubSystemManager {
		public:
			virtual ~ResourceManager()override = default;
			/*初期化*/
			virtual bool Initialize() = 0;
			/*終了処理*/
			virtual void Finalize() = 0;
			/*アンロード対象外のタグを設定*/
			virtual bool SetNotUnloadTags(const std::set<std::string>& tags) = 0;
			/*タグで指定されたリソースをまとめて読み込む*/
			virtual bool PrepareResources(const std::vector<std::string>& need_tag_groups) = 0;
			/*未使用のタリソースをアンロードする*/
			virtual bool UnloadUnusedResouces() = 0;
			/*Resourceの準備が完了したか*/
			virtual bool IsReady()const = 0;
			/*準備進行度(読み込みしていない時は1.0とする)*/
			virtual double GetPrepairProgress()const = 0;
			/*ファイルアクセサをセット。初期化前に呼び出す*/
			virtual void SetFileManipulator_(const std::shared_ptr<FileManipulator>& f_scsr) = 0;
			/*リソースリストファイル名を設定。初期化前に呼び出す必要がある*/
			virtual void SetResourceListFileName_(const std::string& file_name) = 0;
			/*リソースの属性を追加*/
			template<class C>
			void AddResourceType(const std::string& type_name, const std::string& type_prefix) {
				AddResourceCreator(typeid(C), type_name, type_prefix, []()->std::shared_ptr<ResourceBase> {
					return MakeResource<C>();
				});
			}
		protected:
			/*リソースクリエータ関数型*/
			using ResourceCreatorType = std::function<std::shared_ptr<ResourceBase>()>;
		private:
			/*リソース用shared_ptr作成*/
			template<class Res>
			static std::shared_ptr<Res> MakeResource() {
				static_assert(std::is_base_of<ResourceBase, Res>::value == true, "Res is not derived ResourceBase.");
				return std::move(std::shared_ptr<Res>(new Res(), [](Res* r)->void {r->Dispose(); delete r; }));
			}
			virtual void AddResourceTypeProc(const std::type_info& type, const std::string& type_name, const std::string& type_prefix, const ResourceCreatorType& creator) = 0;
		};
	}
}
