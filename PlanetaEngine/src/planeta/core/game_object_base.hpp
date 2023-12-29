#pragma once

#include "Object.hpp"
#include "game_object_component_holder.hpp"
#include "i_game_object.hpp"
#include "NonCopyable.hpp"
#include "game_object_manager_connection.hpp"

namespace plnt {
	class JSONObject;
	class game_object_component;
	class go_component_getter;

	namespace private_ {
		class i_scene_internal;

		/*! @brief GameObjectの具体的な実装を行うクラス。直接用いることはない。
		*/
		class game_object_base final : public Object, public i_game_object, util::NonCopyable<game_object_base>,
		                               public std::enable_shared_from_this<game_object_base> {
		public:
			using super = Object;
			game_object_base();
			game_object_base(const game_object_base &) = delete;
			game_object_base(game_object_base &&) = delete;
			~game_object_base() override;
			game_object_base &operator=(const game_object_base &) = delete;
			game_object_base &operator=(game_object_base &&) = delete;

			//インターフェイスのオーバーライド
			//有効化する
			void activate() override;
			//無効化する
			void inactivate() override;
			//破棄する
			void dispose() override;
			//自分のstd::shared_ptrを取得する。
			WeakPointer<i_game_object> get_pointer() override;

			//状態を取得
			game_object_state state() const override;
			//シーンへのアクセスを取得
			i_scene &scene() override;

			//システム用関数(Managerから呼び出される｡GameObjectクラスで隠ぺいする)
			//クローン時の処理
			bool process_cloning(const std::shared_ptr<game_object_base> &dst);
			//初期化時の処理
			bool process_initialization();
			//有効化時の処理
			bool process_activation();
			//無効化時の処理
			bool process_inactivation();
			//破棄時の処理
			bool process_disposal();
			//マネージャコネクションをセット
			void set_manager_connection(std::unique_ptr<game_object_manager_connection> &&mgr_conn);
			//シーンをセット
			void set_scene_internal_interface(const WeakPointer<i_scene_internal> &i_scene);
			//コンポーネントリストからコンポーネントを追加し、シーンデータなどをセット
			bool add_and_set_up_components(const std::vector<std::string> &com_type_id_list);
			//boost::ptreeからコンポーネントを作成追加し、シーンデータなどをセット
			bool add_and_set_up_components(const boost::property_tree::ptree &pt);

		private:
			game_object_state state_ = game_object_state::invalid;
			//コンポーネントを型で取得
			std::shared_ptr<game_object_component> get_component_by_type_info(
				const std::type_info &ti,
				const std::function<bool(game_object_component *goc)> &type_checker) const override;
			//コンポーネントを型ですべて取得
			//std::vector<std::shared_ptr<GameObjectComponent>> GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const override final;
			void set_up_attached_task(const WeakPointer<Task> &task) override;

			//マネージャコネクション
			std::unique_ptr<game_object_manager_connection> manager_connection_;
			//シーン内部用インターフェイス
			WeakPointer<i_scene_internal> scene_internal_interface_;
			//コンポーネントホルダー
			game_object_component_holder component_holder_;

			//コンポーネント関連の処理
			void set_scene_and_go_data_to_components();
			void set_scene_and_go_data_to_component(game_object_component &com);
			//コンポーネントTypeIDからコンポーネントを追加。
			bool add_components_from_type_id_list(const std::vector<std::string> &com_type_id_list,
			                                      std::vector<std::shared_ptr<game_object_component>> &added_coms);
			//コンポーネントをPtreeから設定
			static bool set_data_to_components_from_ptree(const std::vector<std::shared_ptr<game_object_component>> &coms,
			                                              const std::vector<std::shared_ptr<const
				                                              boost::property_tree::ptree>> &
			                                              pts);

			//アタッチされたタスク
			std::list<WeakPointer<Task>> attached_tasks_;
			//アタッチされたが存在しるか確認しつつ、タスクに処理を行う。存在しない場合はリストから外す。
			bool check_and_apply_process_to_attached_task(const std::function<bool(Task &)> &proc);
		};

		PE_REFLECTABLE_CLASS(game_object_base);
	}
}
