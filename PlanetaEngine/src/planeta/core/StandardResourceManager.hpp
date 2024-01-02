#pragma once

#include <vector>
#include <set>
#include <unordered_map>
#include <memory>
#include <list>
#include <typeindex>

#include "boost/type_index.hpp"

#include "log_utility.hpp"
#include "resource_manager.hpp"


namespace plnt {
	class file;

	namespace private_ {
		class resource_manager_internal_accessor;

		/// <summary>標準のResource管理クラス。非同期読み込みは未実装だが、実装時に変更が必要ないように非同期読み込みと同じように扱う。
		///		PrepareResourcesでリソースの準備を開始し、IsReadyがtrueになるまで待つ。そしてUnloadUnusedResourcesを呼ぶ。
		///			現状ではIsReadyは常にtrue、GetPrepareProgressは常に1.0を返す。</summary>
		class standard_resource_manager final : public resource_manager {
		public:
			/// <summary>Default constructor.</summary>
			standard_resource_manager();
			standard_resource_manager(const standard_resource_manager &) = delete;
			standard_resource_manager(standard_resource_manager &&) = delete;
			/// <summary>Destructor.</summary>
			~standard_resource_manager() override;
			standard_resource_manager &operator=(const standard_resource_manager &) = delete;
			standard_resource_manager &operator=(standard_resource_manager &&) = delete;

			/// <summary>Initializes this object.</summary>
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool initialize() override;
			/// <summary>Finalizes this object.</summary>
			void finalize() override;
			/// <summary>アンロード対象外のタグを設定</summary>
			/// <param name="tags">The tags.</param>
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool set_not_unload_tags(const std::set<std::string> &tags) override;
			/// <summary>タグで指定されたリソースをまとめて読み込む</summary>
			/// <param name="need_tag_groups">Groups the need tag belongs to.</param>
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool prepare_resources(const std::vector<std::string> &need_tag_groups) override;
			/// <summary>未使用のタリソースをアンロードする</summary>
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool unload_unused_resources() override;
			/// <summary>Resourceの準備が完了したか</summary>
			/// <returns>True if ready, false if not.</returns>
			[[nodiscard]] bool is_ready() const override;
			/// <summary>準備進行度(読み込みしていない時は1.0とする)</summary>
			/// <returns>The prepare progress.</returns>
			[[nodiscard]] double get_prepare_progress() const override;
			/// <summary>リソースをIDで取得</summary>
			/// <param name="type">The type.</param>
			/// <param name="id">The identifier.</param>
			/// <returns>The resource by type and identifier.</returns>
			[[nodiscard]] std::shared_ptr<resource_base>
			get_resource_by_type_and_id(const std::type_info &type, const std::string &id) override;
			/// <summary>リソースをFullIDで取得</summary>
			/// <param name="full_id">Identifier for the full.</param>
			/// <returns>The resource by full identifier.</returns>
			[[nodiscard]] std::shared_ptr<resource_base> get_resource_by_full_id(const std::string &full_id) override;
			/// <summary>ファイルアクセサをセット。初期化前に呼び出す</summary>
			/// <param name="f_manipulator">The manipulator.</param>
			void set_file_manipulator(const std::shared_ptr<file_manipulator> &f_manipulator) override;
			/// <summary>リソースリストファイル名を設定。初期化前に呼び出す必要がある</summary>
			/// <param name="file_name">Filename of the file.</param>
			void set_resource_list_file_name(const std::string &file_name) override;

		private:
			/// <summary>Executes the resource type added action.</summary>
			/// <param name="type">The type.</param>
			/// <param name="type_name">Name of the type.</param>
			/// <param name="type_prefix">The type prefix.</param>
			/// <param name="creator">The creator.</param>
			void on_resource_type_added(const std::type_info &type, const std::string &type_name,
			                            const std::string &type_prefix, const resource_creator_type &creator) override;

			/// <summary>The file accessor.</summary>
			std::shared_ptr<file_manipulator> file_accessor_;
			/// <summary>リソースリストのファイル名</summary>
			std::string resource_list_file_name_;

			/// <summary>A resource data.</summary>
			struct resource_data {
				/// <summary>種類</summary>
				std::type_index type_index;
				/// <summary>ID</summary>
				std::string id;
				/// <summary>フルID</summary>
				std::string full_id;
				/// <summary>ファイルパス</summary>
				std::string file_path;
				/// <summary>メタデータファイルパス</summary>
				std::string metadata_file_path;
				/// <summary>リソース本体</summary>
				std::shared_ptr<resource_base> resource;
				/// <summary>リソースがロードされているか</summary>
				bool is_loaded = false;
				/// <summary>タグ</summary>
				std::vector<std::string> tags;
				/// <summary>アンロード対象外か</summary>
				bool not_unload = false;
				/// <summary>True if this object has metadata.</summary>
				bool has_metadata = false;
			};

			/// <summary>Type of the resource data list.</summary>
			using resource_data_list_type = std::list<resource_data>;
			/// <summary>List of resource data.</summary>
			resource_data_list_type resource_data_list_;
			/// <summary>The tag map.</summary>
			std::unordered_map<std::string, std::vector<resource_data_list_type::iterator>> tag_map_;
			/// <summary>The full identifier map.</summary>
			std::unordered_map<std::string, resource_data_list_type::iterator> full_id_map_;
			/// <summary>The path map.</summary>
			std::unordered_map<std::string, resource_data_list_type::iterator> path_map_;
			/// <summary>The not unload tags.</summary>
			std::set<std::string> not_unload_tags_;

			/// <summary>A resource type data.</summary>
			struct resource_type_data {
				/// <summary>The type.</summary>
				boost::typeindex::type_index type;
				/// <summary>Name of the type.</summary>
				std::string type_name;
				/// <summary>The type prefix.</summary>
				std::string type_prefix;
				/// <summary>The creator.</summary>
				resource_creator_type creator;
			};

			/// <summary>ResourceのタイプによるResourceタイプデータ</summary>
			std::unordered_map<std::type_index, resource_type_data> resource_type_data_map_;
			/// <summary>リソースタイプ接頭辞によるリソースタイプマップ</summary>
			std::unordered_map<std::string, std::type_index> resource_type_prefix_to_type_map_;
			/// <summary>リソースの作成</summary>
			/// <param name="type">The type.</param>
			/// <returns>The new resource.</returns>
			[[nodiscard]] std::shared_ptr<resource_base> create_resource(const std::type_index &type);
			/// <summary>リソースのロード</summary>
			/// <param name="res_dat">[in,out] The resource dat.</param>
			/// <returns>The resource.</returns>
			[[nodiscard]] std::shared_ptr<resource_base> load_resource(resource_data &res_dat);
			/// <summary>型とIDからFullIDを取得</summary>
			/// <param name="type_index">Zero-based index of the type.</param>
			/// <param name="id">The identifier.</param>
			/// <returns>The full identifier from type and identifier.</returns>
			[[nodiscard]] std::string get_full_id_from_type_and_id(const boost::typeindex::type_index &type_index,
			                                                       const std::string &id);
			/// <summary>リソースのアンロード</summary>
			/// <param name="res_dat">[in,out] The resource dat.</param>
			static void unload_resource(resource_data &res_dat);
			/// <summary>リソースデータの登録</summary>
			/// <param name="res_dat">[in,out] The resource dat.</param>
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool register_resource_data(resource_data &&res_dat);
			/// <summary>アンロードタグの指定</summary>
			/// <param name="tags">The tags.</param>
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool set_not_unload_tags_impl(const std::set<std::string> &tags);
			/// <summary>リソースリストの読み込み</summary>
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool load_resource_list();
			/// <summary>読み込まれているすべてのリソースをアンロードする</summary>
			void unload_all_loaded_resources();
			/// <summary>内部マネージャアクセサを取得</summary>
			/// <returns>The new internal manager accessor.</returns>
			[[nodiscard]] resource_manager_internal_accessor create_internal_manager_accessor();
			/// <summary>リソースをIDで取得する。ロードされていないリソース指定時に警告を出すか指定可能</summary>
			/// <param name="full_id">Identifier for the full.</param>
			/// <param name="is_valid_not_preload_warning">True if this object is valid not preload warning.</param>
			/// <returns>The resource by full identifier.</returns>
			[[nodiscard]] std::shared_ptr<resource_base> get_resource_by_full_id(const std::string &full_id,
				bool is_valid_not_preload_warning);
			/// <summary>リソースをパスで取得する。ロードされていないリソース指定時に警告を出すか指定可能</summary>
			/// <param name="path">Full pathname of the file.</param>
			/// <param name="root_path">Full pathname of the root file.</param>
			/// <param name="is_valid_not_preload_warning">True if this object is valid not preload warning.</param>
			/// <returns>The resource by path.</returns>
			[[nodiscard]] std::shared_ptr<resource_base> get_resource_by_path(
				const std::string &path, const std::string &root_path,
				bool is_valid_not_preload_warning);
		};
	}
}
