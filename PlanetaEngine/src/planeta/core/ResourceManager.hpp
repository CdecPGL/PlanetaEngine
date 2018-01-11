#pragma once

#include <set>
#include <vector>
#include <functional>
#include "planeta/core/IResourceManager.hpp"
#include "SubSystemManager.hpp"

namespace plnt {
	class FileManipulator;
	namespace private_ {
		/// <summary>リソースマネージャ.</summary>
		class ResourceManager : public IResourceManager, public SubSystemManager {
		public:
			/// <summary>Destructor.</summary>
			virtual ~ResourceManager()override = default;
			/// <summary>初期化</summary>
			/// <returns>True if it succeeds, false if it fails.</returns>
			virtual bool Initialize() = 0;
			/// <summary>終了処理</summary>
			virtual void Finalize() = 0;
			/// <summary>アンロード対象外のタグを設定</summary>
			/// <param name="tags">The tags.</param>
			/// <returns>True if it succeeds, false if it fails.</returns>
			virtual bool SetNotUnloadTags(const std::set<std::string>& tags) = 0;
			/// <summary>タグで指定されたリソースをまとめて読み込む</summary>
			/// <param name="need_tag_groups">Groups the need tag belongs to.</param>
			/// <returns>True if it succeeds, false if it fails.</returns>
			virtual bool PrepareResources(const std::vector<std::string>& need_tag_groups) = 0;
			/// <summary>未使用のタリソースをアンロードする</summary>
			/// <returns>True if it succeeds, false if it fails.</returns>
			virtual bool UnloadUnusedResouces() = 0;
			/// <summary>Resourceの準備が完了したか</summary>
			/// <returns>True if ready, false if not.</returns>
			virtual bool IsReady()const = 0;
			/// <summary>準備進行度(読み込みしていない時は1.0とする)</summary>
			/// <returns>The prepair progress.</returns>
			virtual double GetPrepairProgress()const = 0;
			/// <summary>ファイルアクセサをセット。初期化前に呼び出す</summary>
			/// <param name="f_scsr">The scsr.</param>
			virtual void SetFileManipulator_(const std::shared_ptr<FileManipulator>& f_scsr) = 0;
			/// <summary>リソースリストファイル名を設定。初期化前に呼び出す必要がある</summary>
			/// <param name="file_name">Filename of the file.</param>
			virtual void SetResourceListFileName_(const std::string& file_name) = 0;
			/// <summary>リソースの属性を追加</summary>
			/// <param name="type_name">Name of the type.</param>
			/// <param name="type_prefix">The type prefix.</param>
			template<class C>
			void AddResourceType(const std::string& type_name, const std::string& type_prefix) {
				OnResourceTypeAdded(typeid(C), type_name, type_prefix, []()->std::shared_ptr<ResourceBase> {
					return MakeResource<C>();
				});
			}
		protected:
			/// <summary>リソースクリエータ関数型</summary>
			using ResourceCreatorType = std::function<std::shared_ptr<ResourceBase>()>;
		private:
			/// <summary>リソース用shared_ptr作成</summary>
			/// <returns>A std::shared_ptr&lt;Res&gt;</returns>
			template<class Res>
			static std::shared_ptr<Res> MakeResource() {
				static_assert(std::is_base_of<ResourceBase, Res>::value == true, "Res is not derived ResourceBase.");
				return std::move(std::shared_ptr<Res>(new Res(), [](Res* r)->void {r->Dispose(); delete r; }));
			}
			/// <summary>Executes the resource type added action.</summary>
			/// <param name="type">The type.</param>
			/// <param name="type_name">Name of the type.</param>
			/// <param name="type_prefix">The type prefix.</param>
			/// <param name="creator">The creator.</param>
			virtual void OnResourceTypeAdded(const std::type_info& type, const std::string& type_name, const std::string& type_prefix, const ResourceCreatorType& creator) = 0;
		};
	}
}
