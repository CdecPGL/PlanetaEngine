#pragma once

#include <set>
#include <vector>
#include <functional>

#include "i_resource_manager.hpp"
#include "sub_system_manager.hpp"

namespace plnt {
	class file_manipulator;

	namespace private_ {
		/// <summary>リソースマネージャ.</summary>
		class resource_manager : public i_resource_manager, public sub_system_manager {
		public:
			resource_manager() = default;
			resource_manager(const resource_manager &) = delete;
			resource_manager &operator=(const resource_manager &) = delete;
			/// <summary>Destructor.</summary>
			~resource_manager() override = default;
			resource_manager(resource_manager &&) = delete;
			resource_manager &operator=(resource_manager &&) = delete;

			/// <summary>初期化</summary>
			/// <returns>True if it succeeds, false if it fails.</returns>
			virtual bool initialize() = 0;
			/// <summary>終了処理</summary>
			virtual void finalize() = 0;
			/// <summary>アンロード対象外のタグを設定</summary>
			/// <param name="tags">The tags.</param>
			/// <returns>True if it succeeds, false if it fails.</returns>
			virtual bool set_not_unload_tags(const std::set<std::string> &tags) = 0;
			/// <summary>タグで指定されたリソースをまとめて読み込む</summary>
			/// <param name="need_tag_groups">Groups the need tag belongs to.</param>
			/// <returns>True if it succeeds, false if it fails.</returns>
			virtual bool prepare_resources(const std::vector<std::string> &need_tag_groups) = 0;
			/// <summary>未使用のタリソースをアンロードする</summary>
			/// <returns>True if it succeeds, false if it fails.</returns>
			virtual bool unload_unused_resources() = 0;
			/// <summary>Resourceの準備が完了したか</summary>
			/// <returns>True if ready, false if not.</returns>
			[[nodiscard]] virtual bool is_ready() const = 0;
			/// <summary>準備進行度(読み込みしていない時は1.0とする)</summary>
			/// <returns>The prepare progress.</returns>
			[[nodiscard]] virtual double get_prepare_progress() const = 0;
			/// <summary>ファイルアクセサをセット。初期化前に呼び出す</summary>
			/// <param name="f_accessor">The accessor.</param>
			virtual void set_file_manipulator(const std::shared_ptr<file_manipulator> &f_accessor) = 0;
			/// <summary>リソースリストファイル名を設定。初期化前に呼び出す必要がある</summary>
			/// <param name="file_name">Filename of the file.</param>
			virtual void set_resource_list_file_name(const std::string &file_name) = 0;
			/// <summary>リソースの属性を追加</summary>
			/// <param name="type_name">Name of the type.</param>
			/// <param name="type_prefix">The type prefix.</param>
			template <class C>
			void add_resource_type(const std::string &type_name, const std::string &type_prefix) {
				on_resource_type_added(typeid(C), type_name, type_prefix, []()-> std::shared_ptr<resource_base> {
					return make_resource<C>();
				});
			}

		protected:
			/// <summary>リソースクリエータ関数型</summary>
			using resource_creator_type = std::function<std::shared_ptr<resource_base>()>;

		private:
			/// <summary>リソース用shared_ptr作成</summary>
			/// <returns>A std::shared_ptr&lt;Res&gt;</returns>
			template <class Res>
			[[nodiscard]] static std::shared_ptr<Res> make_resource() {
				static_assert(std::is_base_of_v<resource_base, Res> == true, "Res is not derived ResourceBase.");
				return std::move(std::shared_ptr<Res>(new Res(), [](Res *r)-> void {
					r->dispose();
					delete r;
				}));
			}

			/// <summary>Executes the resource type added action.</summary>
			/// <param name="type">The type.</param>
			/// <param name="type_name">Name of the type.</param>
			/// <param name="type_prefix">The type prefix.</param>
			/// <param name="creator">The creator.</param>
			virtual void on_resource_type_added(const std::type_info &type, const std::string &type_name,
			                                    const std::string &type_prefix,
			                                    const resource_creator_type &creator) = 0;
		};
	}
}
